

       /************************************************************
        * @file   computeshader.cpp
        * @author Evan Gray 
        * 
        * @brief: Abstracts openGL compute shaders
       *************************************************************/
       #include "klpch.h"
       
       //Local header files
       #include "ComputeShader.h"
       #include "Renderer.h"
       
       //Global header files
       #include &ltfstream>
       #include &ltsstream>
       #include &ltglad/glad.h>
       
       namespace Engine
       {
         /*********************************************************************
          * @brief: Creates a compute shader program from a file
          * 
          * @param  filepath: The path to the file containing the shader
         *********************************************************************/
         ComputeShader::ComputeShader(const std::string& filepath)
           : mFilePath(filepath), mComputeShaderID(0)
       	 {
       	   //Grab shaders from file
       	   std::string source = ParseShader();
        
       	   //Set up shader
       	   mComputeShaderID = CreateShader(source);
       	 }
        
         /*********************************************************************
          * @brief: Deletes this compute shader
         *********************************************************************/
       	 ComputeShader::~ComputeShader()
       	 {
       	   glDeleteProgram(mComputeShaderID);
       	 }
        
         //--------------------------------------------------------------------------------------
         //Member functions----------------------------------------------------------------------
         //--------------------------------------------------------------------------------------
         /*********************************************************************
          * @brief: Dispatches this compute shader
          * 
          * @param  groupDimensions: x, y, z sizes of the work group
          * @param  memoryBarrierType: opengl memory barrier type code (use
          *                           their defines), pass 0 for no barrier
         *********************************************************************/
       	 void ComputeShader::Dispatch(glm::vec3 groupDimensions, int memoryBarrierType) const
       	 {
       	   glDispatchCompute(groupDimensions.x, groupDimensions.y, groupDimensions.z);
        
           //If memory barrier is needed
       	   if (memoryBarrierType != 0)
       	   {
       	     glMemoryBarrier(memoryBarrierType);
       	   }
       	 }
        
         /*********************************************************************
          * @brief: Binds this buffer for OpenGL rendering
         *********************************************************************/
       	 void ComputeShader::Bind() const
       	 {
       	   glUseProgram(mComputeShaderID);
       	 }
        
         /********************************************************************* 
          * @brief: Binds texture to compute shader
          * 
          * @param  textureID: Id of texture to bind
          * @param  index: Index of texture in compute shader
          * @param  layered: If this texture is layered
         *********************************************************************/
       	 void ComputeShader::BindTexture(unsigned int textureID, int index, bool layered) const
       	 {
       	   glBindImageTexture(index, textureID, 0, layered, 0, GL_READ_ONLY, GL_RGBA8);
       	 }
        
         /*********************************************************************
          * @brief: Makes a buffer and sets the id
          * 
          * @param  bufferID: Will be filled with id of this buffer
         *********************************************************************/
       	 void ComputeShader::GenBuffer(unsigned int* bufferID) const
       	 {
       	   //Generate buffer
       	   glGenBuffers(1, bufferID);
       	   glBindBuffer(GL_SHADER_STORAGE_BUFFER, *bufferID);
       	 }
        
         /*********************************************************************
          * @brief: Binds a buffer's data to the compute shader
          * 
          * @param  bufferID: Id of buffer to bind
          * @param  index: Index of buffer in compute shader
          * @param  data: Array data to bind
          * @param  size: Size of array to bind
          * @param  flag: Flag for buffer
         *********************************************************************/
       	 void ComputeShader::BindBuffer(unsigned int* bufferID, 
                                        int index, 
                                        float* data, 
                                        int size, 
                                        ComputeShader::flags flag) const
       	 {
       	   glBindBuffer(GL_SHADER_STORAGE_BUFFER, *bufferID);
        
       	   //Set buffer data
       	   glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * size, data, flag);
        
       	   //Send data to compute
       	   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, *bufferID);
       	 }
        
         /*********************************************************************
          * @brief: Reads data from a buffer on the pu
          * 
          * @param  bufferID: Id of buffer to read from
          * @param  data: Will be filled with read data
          * @param  size: Size of array to fill
         *********************************************************************/
       	 void ComputeShader::ReadBuffer(unsigned int* bufferID, float** data, int size) const
       	 {
       	   glBindBuffer(GL_SHADER_STORAGE_BUFFER, *bufferID);
        
       	   //Get data from compute
       	   float* updatedData = static_cast&ltfloat*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
       	   if (updatedData) 
       	   {
       	     for (int i = 0; i &lt size; ++i)
       	     {
               (*data)[i] = updatedData[i];
       	     }
        
       	     glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
       	   }
       	 }
        
         /*********************************************************************
          * @brief: Deletes a buffer created by GenBuffer
          * 
          * @param  bufferID: Id of buffer to delete
         *********************************************************************/
       	 void ComputeShader::DeleteBuffer(unsigned int* bufferID) const
       	 {
       	   //Delete buffer
       	   glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
       	   glDeleteBuffers(1, bufferID);
       	 }
        
         /*********************************************************************
          * @brief: Unbinds buffer to stop OpenGL rendering
         *********************************************************************/
       	 void ComputeShader::Unbind() const
       	 {
       	   //Turn of this shader and go back to image drawing shader
       	   glUseProgram(0);
       	   RendererGetShader()->Bind();
       	 }
        
         /*********************************************************************
          * @brief: Sets a uniforms by name's information
          * 
          * @param  name:  Name of the uniform to set
          * @param  input: uniform information to set (overloaded to different
          *                types)
         *********************************************************************/
       	 void ComputeShader::SetUniform(const std::string& name, float input) const
       	 {
           //Check if data needs to be updated
       	   if (checkUniformMap(mUniforms, name, &input, 1))
       	   {
       	     try
       	     {
       	       //Send data to gpu
       	       glUniform1f(GetUniformLocation(name), input);
       	     }
       	     catch (...)
             {
       	       KL_ERROR("Compute shader just tried to crash")
             }
       	   }
       	 }

         /*********************************************************************
          * @brief: Sets a uniforms by name's information
          * 
          * @param  name:  Name of the uniform to set
          * @param  input: uniform information to set (overloaded to different
          *                types)
         *********************************************************************/
       	 void ComputeShader::SetUniform(const std::string& name, std::vector&ltfloat>& input) const
       	 {
       	   //Check if data needs to be updated
       	   if(checkUniformMap(mUniforms, name, input.data(), input.size()))
       	   {
       	     try
       	     {
       	       //Send data to gpu
       	       glUniform1fv(GetUniformLocation(name), input.size(), input.data());
       	     }
       	     catch (...)
       	     {
       	       KL_ERROR("Compute shader just tried to crash when setting uniform")
       	     }
       	   }
       	 }

         /*********************************************************************
          * @brief: Sets a uniforms by name's information
          * 
          * @param  name:  Name of the uniform to set
          * @param  input: uniform information to set (overloaded to different
          *                types)
         *********************************************************************/ 
       	 void ComputeShader::SetUniform(const std::string& name, std::vector&ltglm::vec2>& input) const
       	 {
       	   //Convert glm::vec2 to floats
       	   std::vector&ltfloat> inputFloats;
       	   for (const glm::vec2& vec : input) 
       	   {
       	     inputFloats.push_back(vec.x);
       	     inputFloats.push_back(vec.y);
       	   }
             	 	
           //Check if data needs to be updated
       	   if (checkUniformMap(mUniforms, name, inputFloats.data(), inputFloats.size()))
       	   {
       	     try
       	     {
       	       //Send to opengl
       	       glUniform2fv(GetUniformLocation(name), inputFloats.size(), inputFloats.data());
       	     }
       	     catch (...)
       	     {
       	       KL_ERROR("Compute shader just tried to crash when setting uniform")
       	     }
       	   }
       	 }

         /*********************************************************************
          * @brief: Sets a uniforms by name's information
          * 
          * @param  name:  Name of the uniform to set
          * @param  input: uniform information to set (overloaded to different
          *                types)
         *********************************************************************/
       	 void ComputeShader::SetUniform(const std::string& name, std::vector&ltglm::vec4>& input) const
       	 {
       	   //Convert glm::vec2 to floats
       	   std::vector&ltfloat> inputFloats;
       	   for (const glm::vec4& vec : input)
       	   {
       	     inputFloats.push_back(vec.r);
       	     inputFloats.push_back(vec.g);
       	     inputFloats.push_back(vec.b);
       	     inputFloats.push_back(vec.a);
       	   }
        
       	   //Check if data needs to be updated
       	   if (checkUniformMap(mUniforms, name, inputFloats.data(), inputFloats.size()))
       	   {
       	     try
       	     {
       	       //Send to opengl
       	       glUniform4fv(GetUniformLocation(name), inputFloats.size(), inputFloats.data());
       	     }
       	     catch (...)
       	     {
       	       KL_ERROR("Compute shader just tried to crash when setting uniform")
       	     }
       	   }
       	 }
        
         //--------------------------------------------------------------------------------------
         //Helper functions----------------------------------------------------------------------
         //--------------------------------------------------------------------------------------
         /*********************************************************************
          * @brief: Gets a uniform location from a name
          * 
          * @param  name: Name of the uniform to get
          * @return The location of the uniform
        *********************************************************************/
       	 unsigned int ComputeShader::GetUniformLocation(const std::string& name) const
       	 {
       	   //If already in cache
       	   if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
       	   {
             //Return uniform
             return mUniformLocationCache[name];
       	   }
        
       	   //Find uniform in shader
       	   int location = glGetUniformLocation(mComputeShaderID, name.c_str());
       	   if (location == -1)
       	   {
       	     KL_WARN("Warning: uniform {} doesn't exist!", name);
       	   }
       	   else
       	   {
       	     mUniformLocationCache[name] = location;
       	   }
       	   return location;
       	 }
        
         /*********************************************************************
          * @brief: Gets the source code of a compute shader from the filepath 
          *         of this compute shader object
          * 
          * @return The source code of the compute shader
         *********************************************************************/
       	 std::string ComputeShader::ParseShader()
       	 {
       	   //Open file
       	   std::ifstream stream(mFilePath);
        
       	   //While there are new lines in the file
       	   std::string line;
       	   std::stringstream ss;
       	   while (getline(stream, line))
       	   {
       	     //Add line to selected string
       	     ss &lt&lt line &lt&lt '\n';
       	   }
        
       	   //Return formed strings in the struct
       	   return ss.str();
       	 }
        
         /*********************************************************************
          * @brief: Compiles a shader from a string
          * 
          * @param  source: The string the shader code is stored in
          * @return The compiled shader's id
         *********************************************************************/
       	 unsigned int ComputeShader::CompileShader(std::string source)
       	 {
       	   //Creates id for the shader
       	   unsigned int id = glCreateShader(GL_COMPUTE_SHADER);
        
       	   //Get string as a c string for opengl
       	   const char* src = source.c_str();
        
       	   //Set the shader and compile it
       	   glShaderSource(id, 1, &src, nullptr);
       	   glCompileShader(id);
        
       #if defined(_DEBUG) || defined(_EDITORRELEASE)
       	   //Error handling
       	   int result;
       	   glGetShaderiv(id, GL_COMPILE_STATUS, &result);
       	   if (result == GL_FALSE)
       	   {
       	     //Get length of message
       	     int length;
       	     glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        
       	     //Allocate memory for message
       	     char* message = (char*)_malloca(length * sizeof(char));
        
       	     glGetShaderInfoLog(id, length, &length, message);
       	     KL_CRITICAL("Failed to compile compute shader! \n {}", message)
       	     glDeleteShader(id);
       	     return 0;
       	   }
        
       	   KL_INFO("Successfully compiled compute shader")
       #endif
        
       	   return id;
       	 }
        
         /*********************************************************************
          * @brief: Creates a program from a shader string
          * 
          * @param  source: A compute shader in a string
          * @return The Id of the created shader program
         *********************************************************************/
       	 unsigned int ComputeShader::CreateShader(std::string source)
       	 {
       	   //Create ids for shaders and program
       	   unsigned int program = glCreateProgram();
       	   unsigned int cs = CompileShader(source);
        
       	   //Attach shaders 
       	   glAttachShader(program, cs);
       	   glLinkProgram(program);
        
       #if defined(_DEBUG) || defined(_EDITORRELEASE)
       	   //Check for sucessfuly linkage
       	   GLint isLinked = 0;
       	   glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
       	   if (isLinked == GL_FALSE)
       	   {
       	     GLint maxLength = 0;
       	     glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        
       	     //The maxLength includes the NULL character
       	     std::vector&ltGLchar> infoLog(maxLength);
       	     glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        
       	     KL_CRITICAL("Failed to link compute shader! \n {}", infoLog.data())
        
       	     //The program is useless now. So delete it.
       	     glDeleteProgram(program);
        
       	     //Exit with failure.
       	     return 0;
       	   }
        
       	   KL_INFO("Successfully linked compute shader")
       #endif
        
       	   //Finish attaching shaders
       	   glValidateProgram(program);
        
       	   //Can now delete shaders on our side (OpenGL has them)
       	   glDeleteShader(cs);
        
       	   return program;
       	 }
        
         /*********************************************************************
          * @brief: Checks if passed uniform needs to be send to gpu based on
          *         map of saved old states, updates map as well if data is
          *         different
          * 
          * @param  map: Refernece to the map to look through
          * @param  name: Name of the uniform
          * @param  input: Array of values that is the uniform data
          * @param  count: length of input
          * @return True if this uniform needs to be resent to the gpu
         *********************************************************************/
       	 template &lttypename mapType, typename mapValue>
       	 bool ComputeShader::checkUniformMap(mapType& map, std::string name, mapValue* input, int count) const
       	 {
       	   //If not in map
       	   if (map.find(name) == map.end())
       	   {
       	     //Save value in map 
       	     mapValue* inputCopy = new  mapValue[count];
       	     for (int i = 0; i &lt count; ++i)
       	       inputCopy[i] = input[i];
       	     map.emplace(name, std::pair&ltmapValue*, int>(inputCopy, count));
        
       	     //Uniform needs to be updated
       	     return true;
       	   }
       	   else
       	   {
       	     //Check if counts dont match
       	     if (map[name].second != count)
       	     {
       	       //Save value in map 
       	       mapValue* inputCopy = new mapValue[count];
       	       for (int i = 0; i &lt count; ++i)
       	         inputCopy[i] = input[i];
       	       delete[] map[name].first;
       	       map[name] = std::pair&ltmapValue*, int>(inputCopy, count);
        
       	       //Uniform needs to be updated
       	       return true;
       	     }
       	     else
       	     {
       	       //Check if values dont match
       	       bool matching = true;
       	       for (int i = 0; i &lt count; ++i)
       	       {
       	         if (map[name].first[i] != input[i])
       	         {
       	           matching = false;
        
       	           //Update value in map
       	           map[name].first[i] = input[i];
       	         }
       	       }
        
       	       if (!matching)
       	       {
       	         //Uniform needs to be updated
       	         return true;
               }
             }
       	   }
        
       	   //Uniform has not changed
      	   return false;
       	 }
       }