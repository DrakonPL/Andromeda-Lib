#include <Andromeda/Graphics/GL/RenderManagerGL3.h>
#include <Andromeda/Graphics/GL/FrameBufferObjectGL3.h>
#include <Andromeda/Graphics/GL/VertexBufferObjectGL3.h>
#include <Andromeda/Graphics/GL/VertexArrayObjectGL3.h>
#include <Andromeda/Graphics/GL/ShaderGlsl.h>
#include <Andromeda/Graphics/VertexTypes.h>

#include <Andromeda/System/MemoryManager.h>
#include <Andromeda/Utils/Logger.h>


#ifdef ANDROMEDA_GL3

#define GLEW_STATIC
#include <GL/glew.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#endif

#ifdef ANDROMEDA_SWITCH

#include <switch.h>

#include <EGL/egl.h>    // EGL library
#include <EGL/eglext.h> // EGL extensions
#include <glad/glad.h>  // glad library (OpenGL loader)

#endif



namespace Andromeda
{
	namespace Graphics
	{
		RenderManagerGL3::RenderManagerGL3() : RenderManager()
		{
			
		}

#ifdef ANDROMEDA_SWITCH
        bool RenderManagerGL3::InitEgl(NWindow* win)
		{
            Utils::Logger::Instance()->Log("Init egl\n");

            // Connect to the EGL default display
            s_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
            if (!s_display)
            {
                Utils::Logger::Instance()->Log("Could not connect to display! error: %d\n", eglGetError());
                goto _fail0;
            }

            // Initialize the EGL display connection
            eglInitialize(s_display, nullptr, nullptr);

            // Select OpenGL (Core) as the desired graphics API
            if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
            {
                Utils::Logger::Instance()->Log("Could not set API! error: %d", eglGetError());
                goto _fail1;
            }

            // Get an appropriate EGL framebuffer configuration
            EGLConfig config;
            EGLint numConfigs;
            static const EGLint framebufferAttributeList[] =
            {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
                EGL_RED_SIZE,     8,
                EGL_GREEN_SIZE,   8,
                EGL_BLUE_SIZE,    8,
                EGL_ALPHA_SIZE,   8,
                EGL_DEPTH_SIZE,   24,
                EGL_STENCIL_SIZE, 8,
                EGL_NONE
            };
            eglChooseConfig(s_display, framebufferAttributeList, &config, 1, &numConfigs);
            if (numConfigs == 0)
            {
               Utils::Logger::Instance()->Log("No config found! error: %d", eglGetError());
                goto _fail1;
            }

            // Create an EGL window surface
            s_surface = eglCreateWindowSurface(s_display, config, win, nullptr);
            if (!s_surface)
            {
                Utils::Logger::Instance()->Log("Surface creation failed! error: %d", eglGetError());
                goto _fail1;
            }

            // Create an EGL rendering context
            static const EGLint contextAttributeList[] =
            {
                EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
                EGL_CONTEXT_MAJOR_VERSION_KHR, 4,
                EGL_CONTEXT_MINOR_VERSION_KHR, 3,
                EGL_NONE
            };
            s_context = eglCreateContext(s_display, config, EGL_NO_CONTEXT, contextAttributeList);
            if (!s_context)
            {
                Utils::Logger::Instance()->Log("Context creation failed! error: %d", eglGetError());
                goto _fail2;
            }

            // Connect the context to the surface
            eglMakeCurrent(s_display, s_surface, s_surface, s_context);
            return true;

        _fail2:
            eglDestroySurface(s_display, s_surface);
            s_surface = nullptr;
        _fail1:
            eglTerminate(s_display);
            s_display = nullptr;
        _fail0:
            return false;
		}

        void RenderManagerGL3::DeinitEgl()
		{
            if (s_display)
            {
                eglMakeCurrent(s_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
                if (s_context)
                {
                    eglDestroyContext(s_display, s_context);
                    s_context = nullptr;
                }
                if (s_surface)
                {
                    eglDestroySurface(s_display, s_surface);
                    s_surface = nullptr;
                }
                eglTerminate(s_display);
                s_display = nullptr;
            }
		}

#endif

		void RenderManagerGL3::Init()
		{
			#ifdef ANDROMEDA_GL3

			glewExperimental = GL_TRUE;

			//init glwe lib
			glewInit();

			#endif
			
			#ifdef ANDROMEDA_SWITCH

            // Load OpenGL routines using glad
            gladLoadGL();


			#endif
			

			// Define the viewport dimensions
			glViewport(0, 0, _width, _height);

			// Setup OpenGL options
			glEnable(GL_DEPTH_TEST);

			//enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//glEnable(GL_CULL_FACE);
			//glCullFace(GL_BACK);
			//glFrontFace(GL_CCW);

			clearColor[0] = 0.2f;
			clearColor[1] = 0.3f;
			clearColor[2] = 0.3f;
			clearColor[3] = 1.0f;
		}

		void RenderManagerGL3::Finish()
		{
#ifdef ANDROMEDA_SWITCH

            DeinitEgl();

#endif
		}

		void RenderManagerGL3::ClearScreen()
		{
			//glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
			glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void RenderManagerGL3::SetClearColor(unsigned color)
		{
			clearColor[0] = ((color >> 8 * 0) & 0xFF) / 255.0f;
			clearColor[1] = ((color >> 8 * 1) & 0xFF) / 255.0f;
			clearColor[2] = ((color >> 8 * 2) & 0xFF) / 255.0f;
			clearColor[3] = ((color >> 8 * 3) & 0xFF) / 255.0f;
		}

		void RenderManagerGL3::StartFrame()
		{
			
		}

		void RenderManagerGL3::EndFrame()
		{
			
		}

		void RenderManagerGL3::SwapBuffers()
		{
            #ifdef ANDROMEDA_SWITCH

            eglSwapBuffers(s_display, s_surface);

            #endif
		}

		void RenderManagerGL3::CreateTexture(Texture* image)
		{
			if(image != 0)
			{
				glGenTextures(1, image->GetIdPointer());

				// All upcoming GL_TEXTURE_2D operations now have effect on this texture object
				glBindTexture(GL_TEXTURE_2D, image->GetId());
				
				// Set the texture wrapping parameters
				// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
				int wrapType = GL_REPEAT;

				switch (image->GetWrapType())
				{
					case TextureWrapType::ClampToBorder:
						wrapType = GL_CLAMP_TO_BORDER;
						break;
					case TextureWrapType::ClampToEdge:
						wrapType = GL_CLAMP_TO_EDGE;
						break;
					case TextureWrapType::Repeat:
						wrapType = GL_REPEAT;
						break;
					case TextureWrapType::MirroredRepeat:
						wrapType = GL_MIRRORED_REPEAT;
						break;

					default:
						wrapType = GL_REPEAT;
						break;
				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType);

				int textureFormat = GL_RGBA;

				switch (image->GetColorType())
				{
				case TextureColorType::Texture_RGB:
					textureFormat = GL_RGB;
					break;
				case TextureColorType::Texture_RGBA:
					textureFormat = GL_RGBA;
					break;
				default:
					textureFormat = GL_RGBA;
					break;
				}

				//upload texture to gpu memory
				glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, image->GetWidth(), image->GetHeight(), 0, textureFormat, GL_UNSIGNED_BYTE, image->GetImageData());
				
				if (image->GetMipLevel() > 0)
				{
					if (image->GetFilterType() == TextureFilerType::LinearFilter)
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					}
					else
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					}

					//generate mipmaps
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else
				{
					// Set texture filtering parameters
					if (image->GetFilterType() == TextureFilerType::LinearFilter)
					{
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					}
					else
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					}
				}

				// Unbind texture when done, so we won't accidentily mess up our texture.
				glBindTexture(GL_TEXTURE_2D, 0);

				//remove image data
				System::MemoryManager::Instance()->FreeTexture(image);
			}
		}

		void RenderManagerGL3::CreateEmptyTexture(Texture* image)
		{
			if (image != 0)
			{
				glGenTextures(1, image->GetIdPointer());

				// All upcoming GL_TEXTURE_2D operations now have effect on this texture object
				glBindTexture(GL_TEXTURE_2D, image->GetId());

				// Set the texture wrapping parameters
				// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
				int wrapType = GL_REPEAT;

				switch (image->GetWrapType())
				{
				case TextureWrapType::ClampToBorder:
					wrapType = GL_CLAMP_TO_BORDER;
					break;
				case TextureWrapType::ClampToEdge:
					wrapType = GL_CLAMP_TO_EDGE;
					break;
				case TextureWrapType::Repeat:
					wrapType = GL_REPEAT;
					break;
				case TextureWrapType::MirroredRepeat:
					wrapType = GL_MIRRORED_REPEAT;
					break;

				default:
					wrapType = GL_REPEAT;
					break;
				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType);

				int textureFormat = GL_RGBA;

				switch (image->GetColorType())
				{
				case TextureColorType::Texture_RGB:
					textureFormat = GL_RGB;
					break;
				case TextureColorType::Texture_RGBA:
					textureFormat = GL_RGBA;
					break;
				default:
					textureFormat = GL_RGBA;
					break;
				}

				//upload texture to gpu memory
				glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, image->GetWidth(), image->GetHeight(), 0, textureFormat, GL_UNSIGNED_BYTE, 0);
				
 				if (image->GetMipLevel() > 0)
				{
					if (image->GetFilterType() == TextureFilerType::LinearFilter)
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					}
					else
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					}

					//generate mipmaps
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else
				{
					// Set texture filtering parameters
					if (image->GetFilterType() == TextureFilerType::LinearFilter)
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					}
					else
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					}
				}

				// Unbind texture when done, so we won't accidentily mess up our texture.
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		void RenderManagerGL3::DestroyTexture(Texture* image)
		{
			if (image != 0)
			{
				glDeleteTextures(1, image->GetIdPointer());
			}
		}

		void RenderManagerGL3::UseTexture(Texture* image)
		{
			if(image != 0 /*&& _lastTextureId != image->GetId()*/)
			{
				glBindTexture(GL_TEXTURE_2D, image->GetId());
				_lastTextureId = image->GetId();
			}
		}

		void RenderManagerGL3::UpdateTexture(Texture* image, void *data)
		{
			if (image != 0)
			{
				int textureFormat = GL_RGBA;

				switch (image->GetColorType())
				{
				case TextureColorType::Texture_RGB:
					textureFormat = GL_RGB;
					break;
				case TextureColorType::Texture_RGBA:
					textureFormat = GL_RGBA;
					break;
				default:
					textureFormat = GL_RGBA;
					break;
				}

				UseTexture(image);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->GetWidth(), image->GetHeight(), textureFormat, GL_UNSIGNED_BYTE, data);
			}
		}

		void* RenderManagerGL3::CreateVertices(VertexType vertexType, int size)
		{
			switch (vertexType)
			{
			case Simple:
				return new SimpleVertex[size];
				break;
			case Color:
				return new ColorVertex[size];
				break;
			case Textured:
				return new TextureVertex[size];
				break;
			case TextureNormal:
				return new TextureNormalVertex[size];
				break;
			case TextureColor:
				return new TextureColorVertex[size];
				break;
			case ColorNormal:
				return new ColorNormalVertex[size];
				break;
			case TextureColorNormal:
				return new TextureColorVertex[size];
				break;
			case NormalColorWeighJoint:
				return new NormalColorWeighJointVertex[size];
				break;
            case NormalTextureWeighJoint: 
				return new NormalTextureWeighJointVertex[size];
				break;
			default: break;
			}

			return nullptr;
		}		

		VertexArrayObject* RenderManagerGL3::CreateVertexArrayObject(VertexType vertexType, VertexBufferDrawType drawType)
		{
			VertexArrayObjectGL3* _vertexArray = new VertexArrayObjectGL3();

			//set basic info
			_vertexArray->SetVertexBufferDrawType(drawType);
			_vertexArray->SetVertexType(vertexType);
			_vertexArray->SetVertexPrimitive(Triangle);

			return _vertexArray;
		}

		FrameBufferObject*  RenderManagerGL3::CreateFrameBufferObject(int width, int height)
		{
			FrameBufferObjectGL3* fbo = new FrameBufferObjectGL3(width, height);


			return fbo;
		}

		Shader* RenderManagerGL3::CreateShader()
		{
			return new ShaderGlSl();
		}

		std::string RenderManagerGL3::GetVertexShaderName()
		{
			return ".vert";
		}

		std::string RenderManagerGL3::GetFragmnetShaderName()
		{
			return ".frag";
		}

		void RenderManagerGL3::SetDepth(bool state)
		{
			if (state)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
		}

		void RenderManagerGL3::SaveDisplayBuffer(std::string fileName)
		{
			//GLint m_viewport[4];
			//glGetIntegerv(GL_VIEWPORT, m_viewport);

			//
			//glBindTexture(GL_TEXTURE_2D, texture);

			//// rgb image
			//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_viewport[0],
			//	m_viewport[1], m_viewport[2], m_viewport[3], 0);

			//glPixelStorei(GL_PACK_ALIGNMENT, 1);
			//unsigned char *data = new unsigned char[3 * _width * _height];
			//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


			/*uint8_t *pixels = new uint8_t[_width * _height * 4];

			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glReadBuffer(GL_FRONT);
			glReadPixels(0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)pixels);

			gif = new GifWriter();

			if (test1 == 0)
			{
				GifBegin(gif, "test.gif", _width, _height, 0,32);
				test1 = 1;
			}

			if(test1 == 1)
			{
				GifWriteFrame(gif, pixels, _width, _height, 0,32);
				licznik++;

				if (licznik >= 240)
				{
					test1 = 2;
				}
			}

			if (test1 == 2)
			{
				GifEnd(gif);

				test1 = 3;
			}

			delete[] pixels;*/

			//Now the file creation
			//FILE *filePtr = fopen(fileName.c_str(), "wb");
			//if (!filePtr) return;

			//int nSize = _width*_height * 3;

			//unsigned char TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
			//unsigned char header[6] = { _width % 256,_width / 256,
			//	_height % 256,_height / 256,
			//	24,0 };
			//// We write the headers
			//fwrite(TGAheader, sizeof(unsigned char), 12, filePtr);
			//fwrite(header, sizeof(unsigned char), 6, filePtr);
			//// And finally our image data
			//fwrite(pixels, sizeof(GLubyte), nSize, filePtr);
			//fclose(filePtr);

			//free(pixels);

			//glPixelStorei(GL_PACK_ALIGNMENT, 1);
			//glReadPixels(0, 0, _width, _height, GL_BGRA, GL_UNSIGNED_BYTE, data);
			//stbi_write_tga(fileName.c_str(), _width, _height, 3, pixels);
		}
	}
}
