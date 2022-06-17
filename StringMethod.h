
#include <string>

using std::string;

class StringMethod{

    public:

        template <class TDD>
        int getSizeArray(TDD* array);

        template <class TDD>
        void copiarArrays(TDD* arrayOrigen, TDD*& arrayDestino);
        template <class TDD>
        void copiarArrays(TDD* arrayOrigen, TDD*& arrayDestino, int arrayOrigen);

        template <class TDD>
        void concatenarArrays(TDD* arrayOrigen, TDD*& arrayDestino);
        template <class TDD>
        void concatenarArrays(TDD* arrayOrigen, TDD*& arrayDestino, int sizeArrayOrigen, int sizeArrayDestino);

        template <class TDD>
        void insertarEnArray(TDD*& arrayDestino, TDD* characteres, int pos);
		template <class TDD>
        void insertarEnArray(TDD*& arrayDestino, TDD* characteres, int pos, int sizeArrayDestino, int sizeCharacteres);
		
        void copiarCharAString(string& cad, char* c);
        void copiarCharAString(string& cad, char* c, int size);
        
        void copiarStringAChar(char*& c, string cad);
		void copiarStringAChar(char*& c, string cad, int sizeChar);
		
        StringMethod();

};