#include "fwCommon_pch.h"
#include "fwunits.h"


namespace fw
{
	namespace core
	{

		double Units::MMs2Inches(double dMMValue)
		{
			return dMMValue / 25.4;

		}


		double Units::Inches2MMs(double dInches)
		{

			return dInches * 25.4;

		}
	}

}; //namespace