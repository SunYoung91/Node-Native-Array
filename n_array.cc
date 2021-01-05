#include "n_array.h"
#include <napi.h>
#include "macros.h"

#if NAPI_VERSION < 6
;Napi::FunctionReference NArray::constructor;
#endif

Napi::Value NArray::Set(const Napi::CallbackInfo& info) {
    Napi::Env env = this->Env();
    REQUIRE_ARGUMENT_INTEGER(0,index);
    if (info.Length() <= (1) || !info[1].IsObject()) {                        
        Napi::TypeError::New(env, "Argument 1 must be a object").ThrowAsJavaScriptException(); 
        return env.Null(); 
    }  

    size_t tIndex = static_cast<size_t>(index);
    if (tIndex < 0 || tIndex >= length_ ){
        char errorMsg[512] = {0};
        sprintf(errorMsg,"Native Array Set Out Of Bound : %zd\0",tIndex);
        Napi::RangeError::New(env,errorMsg).ThrowAsJavaScriptException(); 
        return env.Null();        
    }

    //检查之前的引用删除引用
    napi_ref currentRef = vec_[tIndex];
    if (currentRef != nullptr) { 
        napi_delete_reference(env,currentRef);
		vec_[tIndex] = nullptr;
    }

    auto status = napi_create_reference(info.Env(),info[1],1, &vec_[tIndex]);
    if (napi_ok != status){
         Napi::TypeError::New(env, "NativeArray Create Object Ref Error").ThrowAsJavaScriptException(); 
         return env.Null(); 
    }

    return Napi::Number::From(env,tIndex);
}

Napi::Value NArray::Get(const Napi::CallbackInfo& info) {
    Napi::Env env = this->Env();
    REQUIRE_ARGUMENT_INTEGER(0,index);

    size_t tIndex = static_cast<size_t>(index);
    if (tIndex < 0 || tIndex >= length_ ){
        char errorMsg[512] = {0};
        sprintf(errorMsg,"Native Array Get Out Of Bound : %zd\0",tIndex);
        Napi::RangeError::New(env,errorMsg).ThrowAsJavaScriptException(); 
        return env.Null();        
    }

    napi_value result;
    napi_get_reference_value(env,vec_[index],&result);
    return Napi::Value::From(env,result);
}

NArray::NArray(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NArray>(info) {
    Napi::Env env = this->Env();
    if (info.Length() <= 0 || !info[0].IsNumber()){
        Napi::TypeError::New(env, "Argument 1 must be a Array Length").ThrowAsJavaScriptException(); 
    } else {
        length_ = info[0].As<Napi::Number>().Int32Value();
		vec_.resize(length_);
		for (auto i = 0; i < vec_.size(); i++) {
			vec_[i] = 0;
		}
        info.This().As<Napi::Object>().DefineProperty(Napi::PropertyDescriptor::Value("length", Napi::Number::New(env, length_), napi_enumerable));
    }
}

NArray::~NArray() {
    auto env = this->Env();
    for (size_t i = 0; i < length_ ;i++){
        //检查之前的引用删除引用
        napi_ref currentRef = vec_[i];
        if (currentRef != nullptr) {
            napi_delete_reference(env,currentRef);
        }      
    }
}

Napi::Object NArray::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);
    Napi::Function t = DefineClass(env, "NativeArray", {
    InstanceMethod("set", &NArray::Set),
    InstanceMethod("get", &NArray::Get)});

#if NAPI_VERSION < 6
    constructor = Napi::Persistent(t);
    constructor.SuppressDestruct();
#else
    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(t);
    env.SetInstanceData<Napi::FunctionReference>(constructor);
#endif

    exports.Set("NativeArray", t);
    return exports;    
}


Napi::Object RegisterModule(Napi::Env env, Napi::Object exports) {
      Napi::HandleScope scope(env);
      NArray::Init(env, exports);
      return exports;
}

NODE_API_MODULE(NativeArray, RegisterModule)

