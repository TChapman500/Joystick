#pragma once

namespace TChapman500 {
namespace Input {

class IOutput
{
public:
	IOutput();
	virtual ~IOutput();


	virtual const wchar_t *GetInterfaceName() = 0;


};

}}