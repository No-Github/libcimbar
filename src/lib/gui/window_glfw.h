/* This code is subject to the terms of the Mozilla Public License, v.2.0. http://mozilla.org/MPL/2.0/. */
#pragma once

#include "gl_2d_display.h"
#include "mat_to_gl.h"
#include "window_interface.h"

#include <GLFW/glfw3.h>
#include <chrono>
#include <string>
#include <thread>

namespace cimbar {

class window_glfw : public window_interface<window_glfw>
{
public:
	window_glfw(unsigned width, unsigned height, std::string title)
	{
		if (!glfwInit())
		{
			_good = false;
			return;
		}

		_w = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (!_w)
		{
			_good = false;
			return;
		}
		glfwMakeContextCurrent(_w);

		_display = std::make_shared<cimbar::gl_2d_display>();
		glGenTextures(1, &_texid);
		init_opengl(width, height);
	}

	~window_glfw()
	{
		if (_w)
			glfwDestroyWindow(_w);
		if (_texid)
			glDeleteTextures(1, &_texid);
		glfwTerminate();
	}

	bool is_good() const
	{
		return _good;
	}

	bool should_close() const
	{
		return glfwWindowShouldClose(_w);
	}

	void rotate(unsigned i=1)
	{
		if (_display)
			_display->rotate(i);
	}

	void show(const cv::Mat& img, unsigned delay)
	{
		std::chrono::time_point start = std::chrono::high_resolution_clock::now();

		if (_display)
		{
			cimbar::mat_to_gl::load_gl_texture(_texid, img);
			_display->draw(_texid);

			swap();
			poll();
		}

		unsigned millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
		if (delay > millis)
			std::this_thread::sleep_for(std::chrono::milliseconds(delay-millis));
	}

protected:
	void init_opengl(int width, int height)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glViewport(0, 0, width, height);
	}

	void swap()
	{
		// show next frame
		glfwSwapBuffers(_w);
	}

	void poll()
	{
		glfwPollEvents();
	}

protected:
	GLFWwindow* _w;
	GLuint _texid;
	std::shared_ptr<cimbar::gl_2d_display> _display;
	bool _good = true;
};

}

