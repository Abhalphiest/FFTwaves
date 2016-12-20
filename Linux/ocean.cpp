#include"ocean.h"

//File: ocean
//Author: Margaret Dorsey
//
// The ocean files provide a class for our FFT ocean.
// Based on code by Keith Lantz.

//sets up everything we need, including GL resources, etc
ocean::ocean(const int N, const float A, const vector2 w, const float length, const bool geometry) :
	g(9.81), geometry(geometry), N(N), Nplus1(N + 1), A(A), w(w), length(length),
	vertices(0), indices(0), h_tilde(0), h_tilde_slopex(0), h_tilde_slopez(0), h_tilde_dx(0), h_tilde_dz(0), fft(0)
{
	//set up all our arrays 
	h_tilde = new complex[N*N];
	h_tilde_slopex = new complex[N*N];
	h_tilde_slopez = new complex[N*N];
	h_tilde_dx = new complex[N*N];
	h_tilde_dz = new complex[N*N];

	//for performing the FFT
	fft = new FFT(N);
	//for the actual plane we'll be deforming
	vertices = new vertex_ocean[Nplus1*Nplus1];
	indices = new unsigned int[Nplus1*Nplus1 * 10];
	w_indices = new unsigned int[Nplus1*Nplus1 * 10];
	int index;

	complex htilde0, htilde0mk_conj;
	//build the plane
	for (int m_prime = 0; m_prime < Nplus1; m_prime++) {
		for (int n_prime = 0; n_prime < Nplus1; n_prime++) {
			index = m_prime * Nplus1 + n_prime;

			htilde0 = hTilde_0(n_prime, m_prime);
			htilde0mk_conj = hTilde_0(-n_prime, -m_prime).conj();

			vertices[index].a = htilde0.a;
			vertices[index].b = htilde0.b;
			vertices[index]._a = htilde0mk_conj.a;
			vertices[index]._b = htilde0mk_conj.b;

			vertices[index].ox = vertices[index].x = (n_prime - N / 2.0f) * length / N;
			vertices[index].oy = vertices[index].y = 0.0f;
			vertices[index].oz = vertices[index].z = (m_prime - N / 2.0f) * length / N;

			vertices[index].nx = 0.0f;
			vertices[index].ny = 1.0f;
			vertices[index].nz = 0.0f;
		}
	}

	//setup indices
	indices_count = 0;
	w_indices_count = 0;
	for (int m_prime = 0; m_prime < N; m_prime++) {
		for (int n_prime = 0; n_prime < N; n_prime++) {
			index = m_prime * Nplus1 + n_prime;

			//wireframe index building
				w_indices[w_indices_count++] = index;				// lines
				w_indices[w_indices_count++] = index + 1;
				w_indices[w_indices_count++] = index;
				w_indices[w_indices_count++] = index + Nplus1;
				w_indices[w_indices_count++] = index;
				w_indices[w_indices_count++] = index + Nplus1 + 1;
				if (n_prime == N - 1) {
					w_indices[w_indices_count++] = index + 1;
					w_indices[w_indices_count++] = index + Nplus1 + 1;
				}
				if (m_prime == N - 1) {
					w_indices[w_indices_count++] = index + Nplus1;
					w_indices[w_indices_count++] = index + Nplus1 + 1;
				}
			
			//shaded geometry index building
				indices[indices_count++] = index;				// two triangles
				indices[indices_count++] = index + Nplus1;
				indices[indices_count++] = index + Nplus1 + 1;
				indices[indices_count++] = index;
				indices[indices_count++] = index + Nplus1 + 1;
				indices[indices_count++] = index + 1;
			
		}
	}

	//setup GL locations and resources
	createProgram(glProgram, glShaderV, glShaderF, "ocean.vert", "ocean.frag");
	vertex = glGetAttribLocation(glProgram, "vertex");
	normal = glGetAttribLocation(glProgram, "normal");
	texture = glGetAttribLocation(glProgram, "texture");
	light_position = glGetUniformLocation(glProgram, "light_position");
	projection = glGetUniformLocation(glProgram, "Projection");
	view = glGetUniformLocation(glProgram, "View");
	model = glGetUniformLocation(glProgram, "Model");

	//load our texture
	
	glActiveTexture(GL_TEXTURE0);
	waterTexture = SOIL_load_OGL_texture
	(
		"watertexture.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y //no need to bother with mip mapping, etc
	);
	glBindTexture(GL_TEXTURE_2D, waterTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	textureLoc = glGetUniformLocation(glProgram, "water");
	
	//pass data to GPU
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_ocean)*(Nplus1)*(Nplus1), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &vbo_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_windices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_windices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, w_indices_count * sizeof(unsigned int), w_indices, GL_STATIC_DRAW);
}

//releases CPU memory
ocean::~ocean() {
	//basically just delete all the arrays we made
	// plus the FFT object
	if (h_tilde)		delete[] h_tilde;
	if (h_tilde_slopex)	delete[] h_tilde_slopex;
	if (h_tilde_slopez)	delete[] h_tilde_slopez;
	if (h_tilde_dx)		delete[] h_tilde_dx;
	if (h_tilde_dz)		delete[] h_tilde_dz;
	if (fft)		delete fft;
	if (vertices)		delete[] vertices;
	if (indices)		delete[] indices;
	if (w_indices) delete[] w_indices;
}
//releases GPU memory
void ocean::release() {
	//vertex buffers
	glDeleteBuffers(1, &vbo_indices);
	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &vbo_windices);
	//shader
	releaseProgram(glProgram, glShaderV, glShaderF);
	//texture
	glDeleteTextures(1, &waterTexture);
}

// deep water dispersion factor from gravity
float ocean::dispersion(int n_prime, int m_prime) {
	//sqrt(gk) transformed into our index space
	float w_0 = (float) 2.0f * M_PI / 200.0f;
	float kx = (float) M_PI * (2 * n_prime - N) / length;
	float kz = (float) M_PI * (2 * m_prime - N) / length;
	return floor(sqrt(g * sqrt(kx * kx + kz * kz)) / w_0) * w_0;
}

// phillips spectrum calculation
float ocean::phillips(int n_prime, int m_prime) {

	//calculating our wind factor
	//function can be found in writeup
	vector2 k(M_PI * (2 * n_prime - N) / length,
		M_PI * (2 * m_prime - N) / length);
	float k_length = k.length();
	if (k_length < 0.000001) return 0.0;

	float k_length2 = k_length  * k_length;
	float k_length4 = k_length2 * k_length2;

	float k_dot_w = k.unit() * w.unit();
	float k_dot_w2 = k_dot_w * k_dot_w * k_dot_w * k_dot_w * k_dot_w * k_dot_w;

	float w_length = w.length();
	float L = w_length * w_length / g;
	float L2 = L * L;

	float damping = 0.001;
	float l2 = L2 * damping * damping;

	return A * exp(-1.0f / (k_length2 * L2)) / k_length4 * k_dot_w2 * exp(-k_length2 * l2);
}

//complex conjugate of our height function
complex ocean::hTilde_0(int n_prime, int m_prime) {
	complex r = gaussianRandomVariable();
	return r * sqrt(phillips(n_prime, m_prime) / 2.0f);
}

//height function
complex ocean::hTilde(float t, int n_prime, int m_prime) {
	int index = m_prime * Nplus1 + n_prime;

	complex htilde0(vertices[index].a, vertices[index].b);
	complex htilde0mkconj(vertices[index]._a, vertices[index]._b);

	float omegat = dispersion(n_prime, m_prime) * t;

	float cos_ = cos(omegat);
	float sin_ = sin(omegat);

	complex c0(cos_, sin_);
	complex c1(cos_, -sin_);

	complex res = htilde0 * c0 + htilde0mkconj * c1;

	return htilde0 * c0 + htilde0mkconj*c1;
}

//displacement vector and normal vector calculations
complex_vector_normal ocean::h_D_and_n(vector2 x, float t) {
	complex h(0.0f, 0.0f);
	vector2 D(0.0f, 0.0f);
	vector3 n(0.0f, 0.0f, 0.0f);

	complex c, res, htilde_c;
	vector2 k;
	float kx, kz, k_length, k_dot_x;

	for (int m_prime = 0; m_prime < N; m_prime++) {
		kz = (float)2.0f * M_PI * (m_prime - N / 2.0f) / length;
		for (int n_prime = 0; n_prime < N; n_prime++) {
			kx = (float)2.0f * M_PI * (n_prime - N / 2.0f) / length;
			k = vector2(kx, kz);

			k_length = k.length();
			k_dot_x = k * x;

			c = complex(cos(k_dot_x), sin(k_dot_x));
			htilde_c = hTilde(t, n_prime, m_prime) * c;

			h = h + htilde_c;

			n = n + vector3(-kx * htilde_c.b, 0.0f, -kz * htilde_c.b);

			if (k_length < 0.000001) continue;
			D = D + vector2(kx / k_length * htilde_c.b, kz / k_length * htilde_c.b);
		}
	}

	n = (vector3(0.0f, 1.0f, 0.0f) - n).unit();

	complex_vector_normal cvn;
	cvn.h = h;
	cvn.D = D;
	cvn.n = n;
	return cvn;
}

//wrapper for FFT transform and updates
void ocean::evaluateWavesFFT(float t) {
	float kx, kz, len, lambda = -1.0f;
	int index, index1;

	for (int m_prime = 0; m_prime < N; m_prime++) {
		kz = (float)M_PI * (2.0f * m_prime - N) / length;
		for (int n_prime = 0; n_prime < N; n_prime++) {
			kx = (float)M_PI*(2 * n_prime - N) / length;
			len = sqrt(kx * kx + kz * kz);
			index = m_prime * N + n_prime;

			h_tilde[index] = hTilde(t, n_prime, m_prime);
			h_tilde_slopex[index] = h_tilde[index] * complex(0, kx);
			h_tilde_slopez[index] = h_tilde[index] * complex(0, kz);
			if (len < 0.000001f) {
				h_tilde_dx[index] = complex(0.0f, 0.0f);
				h_tilde_dz[index] = complex(0.0f, 0.0f);
			}
			else {
				h_tilde_dx[index] = h_tilde[index] * complex(0, -kx / len);
				h_tilde_dz[index] = h_tilde[index] * complex(0, -kz / len);
			}
		}
	}

	for (int m_prime = 0; m_prime < N; m_prime++) {
		fft->fft(h_tilde, h_tilde, 1, m_prime * N);
		fft->fft(h_tilde_slopex, h_tilde_slopex, 1, m_prime * N);
		fft->fft(h_tilde_slopez, h_tilde_slopez, 1, m_prime * N);
		fft->fft(h_tilde_dx, h_tilde_dx, 1, m_prime * N);
		fft->fft(h_tilde_dz, h_tilde_dz, 1, m_prime * N);
	}
	for (int n_prime = 0; n_prime < N; n_prime++) {
		fft->fft(h_tilde, h_tilde, N, n_prime);
		fft->fft(h_tilde_slopex, h_tilde_slopex, N, n_prime);
		fft->fft(h_tilde_slopez, h_tilde_slopez, N, n_prime);
		fft->fft(h_tilde_dx, h_tilde_dx, N, n_prime);
		fft->fft(h_tilde_dz, h_tilde_dz, N, n_prime);
	}

	int sign;
	float signs[] = { 1.0f, -1.0f };
	vector3 n;
	for (int m_prime = 0; m_prime < N; m_prime++) {
		for (int n_prime = 0; n_prime < N; n_prime++) {
			index = m_prime * N + n_prime;		// index into h_tilde..
			index1 = m_prime * Nplus1 + n_prime;	// index into vertices

			sign = (float) signs[(n_prime + m_prime) & 1];

			h_tilde[index] = h_tilde[index] * (float)sign;

			// height
			vertices[index1].y = h_tilde[index].a;

			// displacement
			h_tilde_dx[index] = h_tilde_dx[index] * (float)sign;
			h_tilde_dz[index] = h_tilde_dz[index] * (float)sign;
			vertices[index1].x = vertices[index1].ox + h_tilde_dx[index].a * lambda;
			vertices[index1].z = vertices[index1].oz + h_tilde_dz[index].a * lambda;

			// normal
			h_tilde_slopex[index] = h_tilde_slopex[index] * sign;
			h_tilde_slopez[index] = h_tilde_slopez[index] * sign;
			n = vector3(0.0f - h_tilde_slopex[index].a, 1.0f, 0.0f - h_tilde_slopez[index].a).unit();
			vertices[index1].nx = n.x;
			vertices[index1].ny = n.y;
			vertices[index1].nz = n.z;

			// for tiling
			if (n_prime == 0 && m_prime == 0) {
				vertices[index1 + N + Nplus1 * N].y = h_tilde[index].a;

				vertices[index1 + N + Nplus1 * N].x = vertices[index1 + N + Nplus1 * N].ox + h_tilde_dx[index].a * lambda;
				vertices[index1 + N + Nplus1 * N].z = vertices[index1 + N + Nplus1 * N].oz + h_tilde_dz[index].a * lambda;

				vertices[index1 + N + Nplus1 * N].nx = n.x;
				vertices[index1 + N + Nplus1 * N].ny = n.y;
				vertices[index1 + N + Nplus1 * N].nz = n.z;
			}
			if (n_prime == 0) {
				vertices[index1 + N].y = h_tilde[index].a;

				vertices[index1 + N].x = vertices[index1 + N].ox + h_tilde_dx[index].a * lambda;
				vertices[index1 + N].z = vertices[index1 + N].oz + h_tilde_dz[index].a * lambda;

				vertices[index1 + N].nx = n.x;
				vertices[index1 + N].ny = n.y;
				vertices[index1 + N].nz = n.z;
			}
			if (m_prime == 0) {
				vertices[index1 + Nplus1 * N].y = h_tilde[index].a;

				vertices[index1 + Nplus1 * N].x = vertices[index1 + Nplus1 * N].ox + h_tilde_dx[index].a * lambda;
				vertices[index1 + Nplus1 * N].z = vertices[index1 + Nplus1 * N].oz + h_tilde_dz[index].a * lambda;

				vertices[index1 + Nplus1 * N].nx = n.x;
				vertices[index1 + Nplus1 * N].ny = n.y;
				vertices[index1 + Nplus1 * N].nz = n.z;
			}
		}
	}
}

//render and update logic
void ocean::render(float t, glm::vec3 light_pos, glm::mat4 Projection, glm::mat4 View, glm::mat4 Model) {

		//do FFT
		evaluateWavesFFT(t);
	
	//standard GL rendering stuff...
	glUseProgram(glProgram);
	glUniform3f(light_position, light_pos.x, light_pos.y, light_pos.z);
	glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(Projection));
	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(View));
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTexture);
	glUniform1i(textureLoc, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_ocean) * Nplus1 * Nplus1, vertices);
	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_ocean), 0);
	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_ocean), (char *)NULL + 12);
	glEnableVertexAttribArray(texture);
	glVertexAttribPointer(texture, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_ocean), (char *)NULL + 24);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry?vbo_windices:vbo_indices);

	//we encode our transformation in the world matrix,
	//which is bad practice in general but works for right now.
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 10; i++) {
			Model = glm::scale(glm::mat4(1.0f), glm::vec3(5.f, 5.f, 5.f));
			Model = glm::translate(Model, glm::vec3(length * i, 0, length * -j));
			glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));
			glDrawElements(geometry ? GL_LINES : GL_TRIANGLES, geometry? w_indices_count:indices_count, GL_UNSIGNED_INT, 0);
		}
	}
}