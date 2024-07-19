// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Channel.h"
#include "Event.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_Channel(void *p = 0);
   static void *newArray_Channel(Long_t size, void *p);
   static void delete_Channel(void *p);
   static void deleteArray_Channel(void *p);
   static void destruct_Channel(void *p);
   static void streamer_Channel(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Channel*)
   {
      ::Channel *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Channel >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Channel", ::Channel::Class_Version(), "Channel.h", 4,
                  typeid(::Channel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Channel::Dictionary, isa_proxy, 16,
                  sizeof(::Channel) );
      instance.SetNew(&new_Channel);
      instance.SetNewArray(&newArray_Channel);
      instance.SetDelete(&delete_Channel);
      instance.SetDeleteArray(&deleteArray_Channel);
      instance.SetDestructor(&destruct_Channel);
      instance.SetStreamerFunc(&streamer_Channel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Channel*)
   {
      return GenerateInitInstanceLocal((::Channel*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Channel*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Event(void *p = 0);
   static void *newArray_Event(Long_t size, void *p);
   static void delete_Event(void *p);
   static void deleteArray_Event(void *p);
   static void destruct_Event(void *p);
   static void streamer_Event(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Event*)
   {
      ::Event *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Event >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Event", ::Event::Class_Version(), "Event.h", 6,
                  typeid(::Event), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Event::Dictionary, isa_proxy, 16,
                  sizeof(::Event) );
      instance.SetNew(&new_Event);
      instance.SetNewArray(&newArray_Event);
      instance.SetDelete(&delete_Event);
      instance.SetDeleteArray(&deleteArray_Event);
      instance.SetDestructor(&destruct_Event);
      instance.SetStreamerFunc(&streamer_Event);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Event*)
   {
      return GenerateInitInstanceLocal((::Event*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Event*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Channel::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Channel::Class_Name()
{
   return "Channel";
}

//______________________________________________________________________________
const char *Channel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Channel*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Channel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Channel*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Channel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Channel*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Channel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Channel*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Event::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Event::Class_Name()
{
   return "Event";
}

//______________________________________________________________________________
const char *Event::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Event*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Event::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Event*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Event::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Event*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Event::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Event*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Channel::Streamer(TBuffer &R__b)
{
   // Stream an object of class Channel.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b.ReadStaticArray((char*)type);
      R__b >> crateID;
      R__b >> moduleID;
      R__b >> channelID;
      R__b >> eventID;
      R__b >> time;
      R__b >> value;
      R__b.CheckByteCount(R__s, R__c, Channel::IsA());
   } else {
      R__c = R__b.WriteVersion(Channel::IsA(), kTRUE);
      R__b.WriteArray(type, 3);
      R__b << crateID;
      R__b << moduleID;
      R__b << channelID;
      R__b << eventID;
      R__b << time;
      R__b << value;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Channel(void *p) {
      return  p ? new(p) ::Channel : new ::Channel;
   }
   static void *newArray_Channel(Long_t nElements, void *p) {
      return p ? new(p) ::Channel[nElements] : new ::Channel[nElements];
   }
   // Wrapper around operator delete
   static void delete_Channel(void *p) {
      delete ((::Channel*)p);
   }
   static void deleteArray_Channel(void *p) {
      delete [] ((::Channel*)p);
   }
   static void destruct_Channel(void *p) {
      typedef ::Channel current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Channel(TBuffer &buf, void *obj) {
      ((::Channel*)obj)->::Channel::Streamer(buf);
   }
} // end of namespace ROOT for class ::Channel

//______________________________________________________________________________
void Event::Streamer(TBuffer &R__b)
{
   // Stream an object of class Event.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> eventID;
      int R__i;
      for (R__i = 0; R__i < 32; R__i++)
         ADC[R__i].Streamer(R__b);
      R__b >> channelFilled;
      R__b.CheckByteCount(R__s, R__c, Event::IsA());
   } else {
      R__c = R__b.WriteVersion(Event::IsA(), kTRUE);
      R__b << eventID;
      int R__i;
      for (R__i = 0; R__i < 32; R__i++)
         ADC[R__i].Streamer(R__b);
      R__b << channelFilled;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Event(void *p) {
      return  p ? new(p) ::Event : new ::Event;
   }
   static void *newArray_Event(Long_t nElements, void *p) {
      return p ? new(p) ::Event[nElements] : new ::Event[nElements];
   }
   // Wrapper around operator delete
   static void delete_Event(void *p) {
      delete ((::Event*)p);
   }
   static void deleteArray_Event(void *p) {
      delete [] ((::Event*)p);
   }
   static void destruct_Event(void *p) {
      typedef ::Event current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Event(TBuffer &buf, void *obj) {
      ((::Event*)obj)->::Event::Streamer(buf);
   }
} // end of namespace ROOT for class ::Event

namespace {
  void TriggerDictionaryInitialization_dict_Impl() {
    static const char* headers[] = {
"Channel.h",
"Event.h",
0
    };
    static const char* includePaths[] = {
"/opt/root-6.22.0/include/",
"/home/Qt/Monitor/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$Channel.h")))  Channel;
class __attribute__((annotate("$clingAutoload$Event.h")))  Event;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Channel.h"
#include "Event.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Channel", payloadCode, "@",
"Event", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dict() {
  TriggerDictionaryInitialization_dict_Impl();
}
