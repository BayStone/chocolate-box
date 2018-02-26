#pragma once

using namespace System;
using namespace System::Collections::Specialized;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace ContentManagement
{
    
/// AssetLinker is used when to retrieve links from objects to other
/// objects and to apply new links. Need for mod package support.
public ref class AssetLinker
{
public:

    virtual ~AssetLinker();

   	virtual NameValueCollection^ CreateLinks();

   	virtual void ApplyLinks(NameValueCollection^ c);

protected:
  
    AssetLinker();
};

}
}