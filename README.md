[![Build Status](https://travis-ci.org/jenszo/CANdb.svg?branch=master)](https://travis-ci.org/jenszo/CANdb) 
<br />

# CANdb
Library for parsing CAN bus database description formats - credits to the people from GENIVI for a great and clean, grammar-based DBC implementation!

For their toolchain they use a rolling release approach pulling in dependencies through submodules (see src/3rdParty).
This is not suitable for my own needs and projects which I prefer to tie to specific release tags of dependencies only.
Furthermore, some of my compilation and installation parameters simply differ from theirs.

*Changes:*
- Building the library with -fPIC
- Allowing installation of Debug and Release variants for bundles distribution
- Distributing small CMake Script to pull in CANdb as a dependancy.
- Versioning (using the release date)
- Removing spdlog dependency to their development branch; replacing it with a cmake find script with release version requirement to avoid linking issues with my own libraries when spdlog changes it's API.

Down the line, the branch 'genivi' is supposed to be merged with changes from genivi's 'master' branch.
I do my work on the 'customized' branch and merge stable revisions into my 'master' and respective tags for future reference.

Changes that could be suitable for a pull request are merged with 'genivi' for pull requests.


