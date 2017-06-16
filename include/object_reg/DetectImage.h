#ifndef DETECTIMAGE_H
#define DETECTIMAGE_H

namespace objectR
{
	class DetectImage
	{
	public:
		DetectImage() {}
		~DetectImage() {}
		virtual void FindObject() {}
		virtual void PrintResult() {}
	};	
}


#endif