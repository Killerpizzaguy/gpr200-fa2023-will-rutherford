#pragma once
unsigned int loadTexture(const char* filePath);
unsigned int loadTexture(const char* filePath, int wrapMode, int filterMode);
unsigned int loadTexture(const char* filePath, int horizontalWrapMode, int verticalWrapMode, int minFilterMode, int magFilterMode);