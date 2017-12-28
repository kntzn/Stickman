#pragma once
#include <string>

class Material
	{
	private:
		std::string name;
		float u;
	
	public:
		Material (std::string NAME, float U)
			{
			name = NAME;
			u = U;
			}
		Material ()
			{	
			}

		float getU () { return u; }
	};

namespace Mat
	{
	Material Rubber ("Rubber", 0.7f);
	}