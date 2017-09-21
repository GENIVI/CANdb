#ifndef VSI_SERIALIZER_HPP_HLKO0MUA
#define VSI_SERIALIZER_HPP_HLKO0MUA

#include <fstream>

struct CANdb_t;

struct VSISerializer {
    VSISerializer(std::ostream& os);
    void operator()(const CANdb_t& db);

private:
    std::ostream& _os;
};


#endif /* end of include guard: VSI_SERIALIZER_HPP_HLKO0MUA */
