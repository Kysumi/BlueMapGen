#pragma once
#include <ChakraCommon.h>
#include <ChakraCore.h>
#include <map>
#include <string>

#include "Helper.h"
#include "ModuleJob.h"

class ModuleHandler
{
	static std::map<std::string, JsModuleRecord> _moduleCache;
	
	static JsErrorCode CALLBACK FetchImportedModule(_In_ JsModuleRecord referencingModule, _In_ JsValueRef specifier, _Outptr_result_maybenull_ JsModuleRecord* dependentModuleRecord) {
		std::string modulePath = specifier.ValueType == JsValueType.String ?
			                         specifier.ToString() : std::string.Empty;
		
		std::string moduleFullPath = GetModuleFullPath(referencingModule, modulePath);


		if (_moduleCache.find(moduleFullPath) == _moduleCache.end())
		{
			dependentModuleRecord = JsModuleRecord.Create(referencingModule, modulePath, moduleFullPath);
			dependentModuleRecord.AddRef();

			_moduleCache[moduleFullPath] = dependentModuleRecord;

			ModuleJob job = new ModuleJob
			{
				Module = dependentModuleRecord,
				Script = std::string.Empty,
				SourceUrl = moduleFullPath,
				IsParsed = false
			};
			_moduleJobQueue.Enqueue(job);
			
		}
		else
		{
			dependentModuleRecord = _moduleCache[moduleFullPath];
		}

		return JsNoError;
	}

	
	JsValueRef EvaluateModuleCode(std::string code, std::string path, JsValueRef* moduleNamespace)
	{
		JsValueRef specifier;
		JsModuleRecord module = JS_INVALID_REFERENCE;
		std::string modulePath = path;
		std::string moduleFullPath = GetModuleFullPath(module, modulePath);

		JsValueRef result;

		JsPointerToString(Helper::stdStringToWString(modulePath).c_str(), 1, &specifier);
		JsInitializeModuleRecord(nullptr, specifier, &module);

		JsSetModuleHostInfo(module, JsModuleHostInfo_FetchImportedModuleCallback, FetchImportedModule);
		JsSetModuleHostInfo(module, JsModuleHostInfo_FetchImportedModuleFromScriptCallback, DynamicImport);
		JsSetModuleHostInfo(module, JsModuleHostInfo_NotifyModuleReadyCallback, Success_NMRC);
		
		// JsModuleRecord module = JsModuleRecord.Create(module, modulePath, moduleFullPath);
		// module.SetFetchImportedModuleCallback(_fetchImportedModuleCallback);
		// module.SetNotifyModuleReadyCallback(_notifyModuleReadyCallback);

		ModuleJob job
		{
			Module = module,
			Script = code,
			SourceUrl = moduleFullPath,
			IsParsed = false
		};
		_moduleJobQueue.Enqueue(job);

		try
		{
			result = EvaluateModulesTree(out moduleNamespace);
			JsValue exception = module.Exception;

			if (exception.IsValid)
			{
				JsValue metadata = JsValue.Invalid;
				if (!JsContext.HasException)
				{
					JsErrorHelpers.SetException(exception);
				}
				metadata = JsContext.GetAndClearExceptionWithMetadata();

				throw JsErrorHelpers.CreateScriptExceptionFromMetadata(metadata);
			}
		}
		finally
		{
			_moduleJobQueue.Clear();
			module.Release();
		}
		

		return result;
	}




















	
	static std::string GetModuleFullPath(JsModuleRecord referencingModule, std::string modulePath)
	{
		LPCWSTR specifierStr;
		size_t length;

		JsErrorCode errorCode = JsStringToPointer(referencingModule, &specifierStr, &length);

		if (errorCode == JsNoError)
		{
			throw "Something went wrong trying to get the file path from the ModuleRecord";
		}
				
		// std::string moduleFullPath = PathHelpers.MakeAbsolutePath(parentModulePath, modulePath);
		// return moduleFullPath;

		std::string parentModulePath = "/";
		
		return parentModulePath;
	}
};

