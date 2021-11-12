#include "crowd_functions.h"

Turtle** create_turtle_crowd(Shader shader, int size) {
	Turtle** turtles = new Turtle * [size];
	turtles[0] = new Turtle(shader);

	for (int i = 1; i < size; i++) {
		// Make copies of the first turtle (boid) and put them in a different location
		turtles[i] = new Turtle(*turtles[0]);
		// Using translation vector for position (= origin (0,0,0) + translation defined by translation_vector)
		turtles[i]->shell.translation_vec.x += rand() % 200 - 100;
		turtles[i]->shell.translation_vec.z += rand() % 200 - 100;
	}
	return turtles;
}

vec3 rule1(Turtle* t, Turtle** crowd, int n) {
	vec3 res = vec3(0, 0, 0);
	for (int i = 0; i < n; i++) {
		if ((crowd[i] == t) && (distance(crowd[i]->shell.translation_vec, t->shell.translation_vec) < 100)) {
			res -= (crowd[i]->shell.translation_vec - t->shell.translation_vec);
		}
	}
	return res;
}

vec3 rule2(Turtle* t, Turtle** crowd, int n) {
	vec3 average = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < n; i++) {
		average += crowd[i]->shell.translation_vec;
	}
	average /= (float)n;

	// Add portion of the average velocity to the boid's velocity
	vec3 res = 0.1f * (average - t->shell.translation_vec);
	return res;
}

vec3 rule3(Turtle* t, Turtle** crowd, int n) {
	vec3 centroid = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < n; i++) {
		centroid += crowd[i]->shell.translation_vec;
	}
	centroid /= (float)n;

	// Move position 2% ? towards the centroid
	vec3 res = 0.05f * (centroid - t->shell.translation_vec);
	return res;
}