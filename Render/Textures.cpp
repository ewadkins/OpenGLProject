/*
 * Textures.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: ericwadkins
 */

#include "Textures.h"
#include "../Application.h"

int Textures::TEXTURE_SLOTS[] = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2,
		GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7,
		GL_TEXTURE8, GL_TEXTURE9, GL_TEXTURE10, GL_TEXTURE11, GL_TEXTURE12,
		GL_TEXTURE13, GL_TEXTURE14, GL_TEXTURE15, GL_TEXTURE16, GL_TEXTURE17,
		GL_TEXTURE18, GL_TEXTURE19, GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22,
		GL_TEXTURE23, GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27,
		GL_TEXTURE28, GL_TEXTURE29, GL_TEXTURE30, GL_TEXTURE31 };

Textures::Textures(Application* application) {
	_application = application;
	_textureCountChanged = false;
	_dynamic = 0;
}

void Textures::initialize() {
	_dynamic = registerTexture();
}

void Textures::update() {
	if (_textureCountChanged) {
		_application->_renderer->createShaders();
		_textureCountChanged = false;
	}
}

int Textures::registerTexture() {
	GLuint id;
	int index = _textureMap.size();
	glActiveTexture(TEXTURE_SLOTS[index]);
	glGenTextures(1, &id);
	_application->_logger->log("Registering empty texture with id ").log((int)id).log(" at index ").log(index).endLine();
	_application->_renderer->_currentProgram->setUniform1i((std::string("textures[") + std::to_string(index) + std::string("]")).c_str(), index);
	_textureMap.push_back(id);
	if (_application->_renderer != nullptr)
		_textureCountChanged = true;
	return index;
}

int Textures::registerTexture(const char* filename) {
	GLuint id = 0;
	int index = _textureMap.size();
	glActiveTexture(TEXTURE_SLOTS[index]);
	std::string fileName = std::string(filename);
	if (fileName.find("png") != std::string::npos)
		id = loadPNGImage(fileName);
	_application->_logger->log("Registering texture frome file ").log(fileName).log(" with id ").log((int)id).log(" at index ").log(index).endLine();
	_application->_renderer->_currentProgram->setUniform1i((std::string("textures[") + std::to_string(index) + std::string("]")).c_str(), index);
	_textureMap.push_back(id);
	if (_application->_renderer != nullptr)
		_textureCountChanged = true;
	return index;
}

void Textures::setUniforms() {
	for (int i = 0; i < _textureMap.size(); i++)
		_application->_renderer->_currentProgram->setUniform1i(
				(std::string("textures[") + std::to_string(i) + std::string("]")).c_str(), i);
}

int Textures::loadPNGImage(std::string filename) {
	//header for testing if it is a png
	png_byte header[8];

	//open file as binary
	FILE *fp = fopen(filename.c_str(), "rb");
	if (!fp) {
		return 0;
	}

	//read the header
	fread(header, 1, 8, fp);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) {
		fclose(fp);
		return 0;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
	NULL, NULL);
	if (!png_ptr) {
		fclose(fp);
		return 0;
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);
		return 0;
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return 0;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}

	//init png reading
	png_init_io(png_ptr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
	NULL, NULL, NULL);

	//update width and height based on png info
	int width = twidth;
	int height = theight;

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data = new png_byte[rowbytes * height];
	if (!image_data) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[height];
	if (!row_pointers) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		fclose(fp);
		return 0;
	}
	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < height; ++i)
		row_pointers[height - 1 - i] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//Now generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, (GLvoid*) image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] image_data;
	delete[] row_pointers;
	fclose(fp);

	return texture;
}

