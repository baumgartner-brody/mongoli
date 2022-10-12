#ifndef _WORLDGEN_H_
#define _WORLDGEN_H_

#include <string>
#include <vector>

/* TODO */
/* Right now these functions belong in a FILESYSTEM namespace */
/*  They are going to be moved */

namespace WorldGen {

    void test();

    int writeToFile(const std::string &fileName, const std::string &data);

    const std::vector<std::string> getDirectories(const std::string &root);
};



#endif /* _WORLDGEN_H_ */