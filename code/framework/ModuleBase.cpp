#include "ModuleBase.hpp"

using namespace std;

namespace uipf
{

ModuleBase::ModuleBase()
:context_(nullptr)
{
}

ModuleBase::~ModuleBase()
{
}

void ModuleBase::setContext(Context* context)
{
	context_ = context;
}

std::string ModuleBase::name() const
{
	return name_;
}


} // namespace

