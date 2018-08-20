#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>

using namespace std;

enum action_type { restart, resume, start, stop, suspend };

inline void stringNotEmpty() {
}

template<typename First, typename ... Strings>
inline void stringNotEmpty(First arg, const Strings&... rest) {
	if (arg != "") {
	}
	else {
		throw invalid_argument("Arguements should not be empty strings.");
	}
	stringNotEmpty(rest...);
}

inline string enumToString(action_type value) {
	switch (value) {
	case restart:
		return "restart";
		break;
	case resume:
		return "resume";
		break;
	case start:
		return "start";
		break;
	case stop:
		return "stop";
		break;
	case suspend:
		return "suspend";
		break;
	}
	return "";
}

inline nlohmann::json notimplemented(string funcName) {
	cout << funcName << " is not implemented yet." << endl;
	nlohmann::json dummy;
	return dummy;
}

inline string makeParam(bool &firstParam, string value, string paramName) {
	if (firstParam && value != "") {
		firstParam = false;
		return "?" + paramName + "=" + value;
	}
	else if (value != "") {
		return "&" + paramName + "=" + value;
	}
	else {
		return "";
	}
}

inline string boolToString(bool b) {
	if (b) {
		return "True";
	}
	else {
		return "False";
	}
}

struct buffer {
	char *ptr;
	size_t len;
};

inline void init_buffer(struct buffer *s) {
	s->len = 0;
	s->ptr = (char*)malloc(s->len + 1);
	if (s->ptr == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(EXIT_FAILURE);
	}
	s->ptr[0] = '\0';
}

inline size_t writefunc(void *ptr, size_t size, size_t nmemb, struct buffer *s) {
	size_t new_len = s->len + size * nmemb;
	s->ptr = (char*)realloc(s->ptr, new_len + 1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->ptr + s->len, ptr, size*nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size * nmemb;
}

#endif