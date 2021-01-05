
#include <vector>
#include <napi.h>

class NArray;
class NArray :public Napi::ObjectWrap<NArray>{
public:
#if NAPI_VERSION < 6
    static Napi::FunctionReference constructor;
#endif

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
  
    //设置下标数据
    Napi::Value Set(const Napi::CallbackInfo& info);

    //获取下标数据
    Napi::Value Get(const Napi::CallbackInfo& info);

    NArray(const Napi::CallbackInfo& info);

    ~NArray();
private:
    size_t length_;
    std::vector<napi_ref> vec_;
};