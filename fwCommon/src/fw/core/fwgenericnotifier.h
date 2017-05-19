#ifndef SIGENERICNOTIFIER_H_INCLUDED
#define SIGENERICNOTIFIER_H_INCLUDED


namespace fw
{
	namespace core
	{

		//base for listener interface handled by SIGenericNotifier
		class GenericListener
		{


		}; //class


		//base class for any object that wants to notify listeners
		//about some events
		class GenericNotifier
		{
			public:
				GenericNotifier();

				virtual void addListener(GenericListener* pListener);
				virtual void removeListener(GenericListener* pListener);

			protected:

				std::list<GenericListener*> m_Listeners;

		}; //class

	} //namespace

}; //namespace

#endif //SIGENERICNOTIFIER_H_INCLUDED