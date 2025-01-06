       
       /*********************************************************************
        * file:   ComputePipeline.cpp
        * author: Evan Gray
        *
        * brief:  Pipeline for running compute shaders
        *********************************************************************/
       #include &ltPCH/pch.h>
       
       #include "ComputePipeline.hpp"
       #include "Backend/Rendering/Uniform/Uniform.hpp"
       #include "Backend/Rendering/Uniform/UniformData.hpp"
       #include "Shader.hpp"
       
       namespace Rendering
       {
         /*********************************************************************
          * @brief: Creates a new Compute pipeline
          * 
          * @param  device: Device this pipeline uses
          * @param  uniforms: ComputePipeline's uniforms
          * @param  computeFilePath: Path to compute shader code file
         *********************************************************************/
       	 ComputePipeline::ComputePipeline(Device& device, 
                                          const std::vector&ltUniform*>& uniforms, 
                                          const std::string& computeFilePath)
       	   : mPipelineDevice(device)
           , mPath(computeFilePath)
       	 {
       	   CreatePipelineLayout(uniforms);
       	   CreatePipeline();
       	 }
        
         /*********************************************************************
          * @brief: Destroys this pipeline object
         *********************************************************************/
       	 ComputePipeline::~ComputePipeline()
       	 {
       	   //Destroy shaders
       	   vkDestroyShaderModule(mPipelineDevice.GetDevice(), mComputeShaderModule, nullptr);
        
       	   //Destroy pipeline
       	   vkDestroyPipeline(mPipelineDevice.GetDevice(), mComputePipeline, nullptr);
        
       	   //Destroy created layout
       	   vkDestroyPipelineLayout(mPipelineDevice.GetDevice(), mPipelineLayout, nullptr);
       	 }

         /*********************************************************************
          * @brief: Binds this pipeline to passed command buffer
          *
          * @param  commandBuffer: Command buffer to bind too
         *********************************************************************/        
       	 void ComputePipeline::Bind(VkCommandBuffer commandBuffer)
       	 {
       	   //Bind the pipeline    
       	   vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, mComputePipeline);
       	 }
        
          /*********************************************************************
           * @brief: Reads file at passed file path into a vector of chars,
           *		     will throw error if filepath cannot be opened
           * 
           * @param  filePath: Path of file to read in
           * @return All characters of the file in a vector
          *********************************************************************/
          std::string ComputePipeline::ReadShader(const std::string& filePath)
          {
            //Read file,       path,     start at end,   read as binaryCompile Shaders
            std::ifstream file{filePath, std::ios::ate | std::ios::binary};

            //Check if not open
            if (!file.is_open())
            {
              std::string error = "Failed to open file: " + filePath;
              NL_CRITICAL(error);
            }

            //Get file size (tellg gets last position, which is end since we started there)
            size_t fileSize = static_cast<size_t>(file.tellg());

            //Make a vector big enough to hold full file
            std::vector<char> buffer(fileSize);

            //Go back to start of file and read whole file into vector
            file.seekg(0);
            file.read(buffer.data(), fileSize);
            file.close();

            std::string shaderSource(buffer.begin(), buffer.end());

            return shaderSource;
          }

         /*********************************************************************
          * @brief:  Creates a shader module object with passed code and sets
          *          passed "shaderModule" to it
          *
          * @param  code: Code to create shader with
          * @param  shaderModule: Gets set to created shader module
         *********************************************************************/
         void ComputePipeline::CreateShaderModule(Device& device, const std::vector<uint32_t>& code, VkShaderModule* shaderModule)
         {
           //Struct to hold information on how to create this shader module
           VkShaderModuleCreateInfo createInfo{};

           //Set type of object to create to shader module
           createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

           //Set the size of the passed code
           createInfo.codeSize = code.size() * sizeof(uint32_t);

           //Set pointer to code (cast from array of chars to a int32 pointer)
           createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

           //Create the shader module
           if (vkCreateShaderModule(device.GetDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
           {
             //Throw error if failed
             NL_CRITICAL("Failed to create shader module");
           }
         }

         /*********************************************************************
          * @brief: Creates the layout for this simpleRenderSystems pipeline
          *
          * @param  uniforms: uniforms of this pipeline
         *********************************************************************/
       	 void ComputePipeline::CreatePipelineLayout(const std::vector&ltUniform*>& uniforms)
       	 {
       	   //Will hold create info for this layout
       	   VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        
       	   //Set what will be created to a pipeline layout
       	   pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        
       	   //Set uniform binding indexes and make vector of discriptions
       	   std::vector&ltVkDescriptorSetLayout> uniformsDescriptorSetLayouts;
       	   for (int i = 0; i &lt uniforms.size(); ++i)
       	   {
       	     uniforms[i]->SetBinding(i);
       	     uniformsDescriptorSetLayouts.push_back(uniforms[i]->GetDescriptorLayout()->GetDescriptorSetLayout());
       	   }
        
       	   //Set how many layouts are being provided and provide the layouts themselves
       	   pipelineLayoutCreateInfo.setLayoutCount = static_cast&ltuint32_t>(uniformsDescriptorSetLayouts.size());
       	   pipelineLayoutCreateInfo.pSetLayouts = uniformsDescriptorSetLayouts.data();
        
       	   //Create pipeline layout
       	   if (vkCreatePipelineLayout(mPipelineDevice.GetDevice(), &pipelineLayoutCreateInfo, nullptr, &mPipelineLayout) 
                != VK_SUCCESS)
       	   {
             NL_CRITICAL("Failed to create pipeline layout");
       	   }
       	 }

         /*********************************************************************
          * @brief: Creates this compute pipeline
         *********************************************************************/
       	 void ComputePipeline::CreatePipeline()
       	 {
       	   //Make sure layout is real
       	   if(mPipelineLayout == VK_NULL_HANDLE)
       	   {
       	     NL_CRITICAL("Cannot create compute pipeline: no pipelineLayout provided");
           }
        
           //Read and compile code file
           const std::string& computeCode = ReadShader(mPath);
           std::vector&ltuint32_t> vertShaderSPV = CompileGLSLtoSPV(computeCode, EShLangCompute);
           CreateShaderModule(mPipelineDevice, vertShaderSPV, &mComputeShaderModule);
        
           //Make create infos for compute shader stage
           VkPipelineShaderStageCreateInfo computeShaderStageInfo{};
           computeShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO; //Set what will be created to a shader module
           computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;                         //Set type to vertex shader
           computeShaderStageInfo.module = mComputeShaderModule;                               //Vertex shader to use
           computeShaderStageInfo.pName = "main";                                              //Name of entry function in vertex shader
           computeShaderStageInfo.flags = 0;                                                   //Using no flags//Using no flags
           computeShaderStageInfo.pNext = nullptr;
           computeShaderStageInfo.pSpecializationInfo = nullptr;
        
           //Create pipeline object using everything we have set up
           VkComputePipelineCreateInfo pipelineCreateInfo{};
           pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO; //Set what will be created to a graphics pipeline
           pipelineCreateInfo.stage = computeShaderStageInfo;                         //Programable shader stages to use
           pipelineCreateInfo.layout = mPipelineLayout;                               //Set data from config
        
           //Create the pipeline 
           if (vkCreateComputePipelines(mPipelineDevice.GetDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &mComputePipeline) 
                != VK_SUCCESS)
           {
             NL_CRITICAL("Failed to create compute pipeline");
           }
       	 }
       }