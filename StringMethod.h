
#include <string>

using std::string;

class StringMethod{

    public:

        template <class TDD>
        int getSizeArray(TDD* array);

        template <class TDD>
        void copiarArrays(TDD* arrayOrigen, TDD*& arrayDestino);

        template <class TDD>
        void concatenarArrays(TDD* arrayOrigen, TDD*& arrayDestino);

        template <class TDD>
        void insertarEnArray(TDD*& arrayDestino, TDD* characteres, int pos);

        void copiarCharAString(string& cad, char* c);
        void copiarStringAChar(char*& c, string cad);

        StringMethod();

};