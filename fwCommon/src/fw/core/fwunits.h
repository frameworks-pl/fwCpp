#ifndef SIUNITS_H_INCLUDED
#define SIUNITS_H_INCLUDED


namespace fw
{
	namespace core
	{
		//unit conversion class
		class Units
		{

		public:

			//converts milimiters to inches
			static double MMs2Inches(double dMMValue);

			//converts inches into milimeters
			static double Inches2MMs(double dInches);

		}; //class

	} //namespace core


}; //namespace

#endif //SIUNITS_H_INCLUDED