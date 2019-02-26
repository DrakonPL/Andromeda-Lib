TARGET_LIB	= libAndromedaLib.a

OBJS   = 	../../Andromeda/FileSystem/FileManager.o \
			../../Andromeda/FileSystem/Sceio/SceioFile.o \
			../../Andromeda/FileSystem/Sceio/SceioDirectory.o \
			../../Andromeda/FileSystem/BaseFile.o \
			../../Andromeda/FileSystem/BaseDirectory.o \
			../../Andromeda/Audio/Sound.o \
			../../Andromeda/Audio/AudioManager.o \
			../../Andromeda/Audio/Vita/VitaSound.o \
			../../Andromeda/Audio/Vita/VitaAudioManager.o \
			../../Andromeda/Audio/Vita/audio_decoder.o \
			../../Andromeda/Audio/Vita/audio_resampler.o \
			../../Andromeda/Audio/Vita/decoder_oggvorbis.o \
			../../Andromeda/Audio/Vita/decoder_wav.o \
			../../Andromeda/Audio/Vita/utils.o \
			../../Andromeda/Graphics/Camera3d.o \
			../../Andromeda/Graphics/Texture.o \
			../../Andromeda/Graphics/RenderManager.o \
			../../Andromeda/Graphics/Shader.o \
			../../Andromeda/Graphics/ShaderManager.o \
			../../Andromeda/Graphics/Sprite.o \
			../../Andromeda/Graphics/TextureManager.o \
			../../Andromeda/Graphics/VertexArrayObject.o \
			../../Andromeda/Graphics/VertexBufferObject.o \
			../../Andromeda/Graphics/FrameBufferObject.o \
			../../Andromeda/Graphics/GXM/RenderManagerGxm.o \
			../../Andromeda/Graphics/GXM/GxmMemoryUtil.o \
			../../Andromeda/Graphics/GXM/ShaderGxm.o \
			../../Andromeda/Graphics/GXM/VertexArrayObjectGxm.o \
			../../Andromeda/Graphics/GXM/VertexBufferObjectGxm.o \
			../../Andromeda/Graphics/GXM/FrameBufferObjectGxm.o \
			../../Andromeda/Graphics/Text/TextureAtlas.o \
			../../Andromeda/Graphics/Text/TexturedFont.o \
			../../Andromeda/Graphics/Text/FontManager.o \
			../../Andromeda/Graphics/Models/ModelObj.o \
			../../Andromeda/Graphics/Models/ModelMaterial.o \
			../../Andromeda/Graphics/Models/ModelMd5.o \
			../../Andromeda/Graphics/Models/ModelMd5Animation.o \
			../../Andromeda/Graphics/Models/ModelMd5Utils.o \
			../../Andromeda/Input/InputManager.o \
			../../Andromeda/Input/Vita/InputManagerVita.o \
			../../Andromeda/Input/Vita/GamepadDeviceVita.o \
			../../Andromeda/Input/Vita/TouchDeviceVita.o \
			../../Andromeda/System/GameLoader.o \
			../../Andromeda/System/GameManager.o \
			../../Andromeda/System/GameState.o \
			../../Andromeda/System/SystemManager.o \
			../../Andromeda/System/MemoryManager.o \
			../../Andromeda/System/Timer.o \
			../../Andromeda/System/GXM/GameLoaderGxm.o \
			../../Andromeda/System/GXM/MemoryManagerGxm.o \
			../../Andromeda/Utils/Logger.o \
			../../Andromeda/Utils/debugnet.o \
			../../Andromeda/Utils/SimpleProfiler.o \
			../../Libs/freetype_gl/distance-field.o \
			../../Libs/freetype_gl/edtaa3func.o \
			../../Libs/freetype_gl/font-manager.o \
			../../Libs/freetype_gl/platform.o \
			../../Libs/freetype_gl/texture-atlas.o \
			../../Libs/freetype_gl/texture-font.o \
			../../Libs/freetype_gl/utf8-utils.o \
			../../Libs/freetype_gl/vector.o \
			../../Libs/tinyxml/tinystr.o \
			../../Libs/tinyxml/tinyxmlerror.o \
			../../Libs/tinyxml/tinyxmlparser.o \
			../../Libs/tinyxml/tinyxml.o \
			../../Libs/tiny_obj_loader/tiny_obj_loader.o
			
SHADERS   = ../../Andromeda/Graphics/GXM/CoreShaders/Compiled/clear_f_gxp.o ../../Andromeda/Graphics/GXM/CoreShaders/Compiled/clear_v_gxp.o
			
PREFIX  	= arm-vita-eabi
CC      	= $(PREFIX)-gcc
CXX      	= $(PREFIX)-g++
STRIP 		= $(PREFIX)-strip
AR      	= $(PREFIX)-ar
CXXFLAGS  	= -Wl,-q -O3 -std=gnu++11 -ffat-lto-objects -flto -fpermissive -DANDROMEDA_VITA -DHAVE_OGGVORBIS -DWANT_FASTWAV -DHAVE_LIBSPEEXDSP -DUSE_AUDIO_RESAMPLER -I../../ -I../../Libs -I../../Libs/glm -I../../Libs/freetype/include -I../../Libs/freetype_gl -I../../Libs/JellyPhysics -I../../Libs/tinyxml -I../../Libs/stb -I../../Libs/nogg -I../../Libs/tiny_obj_loader -I../../Libs/bullet/include/bullet
CFLAGS  	= -Wl,-q -O3 -std=c99 -ffat-lto-objects -flto -DANDROMEDA_VITA -DHAVE_OGGVORBIS -DWANT_FASTWAV  -DHAVE_LIBSPEEXDSP -DUSE_AUDIO_RESAMPLER -I../../ -I../../Libs -I../../Libs/glm -I../../Libs/freetype/include -I../../Libs/freetype_gl -I../../Libs/JellyPhysics -I../../Libs/tinyxml -I../../Libs/stb -I../../Libs/nogg -I../../Libs/tiny_obj_loader -I../../Libs/bullet/include/bullet
ASFLAGS 	= $(CFLAGS)

all: $(TARGET_LIB)

$(TARGET_LIB): $(SHADERS) $(OBJS)
	$(AR) -rc $@ $^

clean:
	@rm -rf $(TARGET_LIB) $(OBJS)
