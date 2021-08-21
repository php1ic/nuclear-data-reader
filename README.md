# Nuclear Data Reader

---

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/php1ic/nuclear-data-reader.svg)](https://github.com/php1ic/nuclear-data-reader/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/php1ic/nuclear-data-reader.svg)](https://github.com/php1ic/nuclear-data-reader/pulls)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![codecov](https://codecov.io/gh/php1ic/nuclear-data-reader/branch/master/graph/badge.svg)](https://codecov.io/gh/php1ic/nuclear-data-reader)
[![CodeFactor](https://www.codefactor.io/repository/github/php1ic/nuclear-data-reader/badge)](https://www.codefactor.io/repository/github/php1ic/nuclear-data-reader)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/5043f510e8cf44efac2b40ea96bcf469)](https://www.codacy.com/gh/php1ic/nuclear-data-reader/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=php1ic/nuclear-data-reader&amp;utm_campaign=Badge_Grade)

---

| OS    | Config                                      | Build Status                                                                                                                                             | Compiler Version |
|-------|---------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|-----------------:|
| Linux | [Github](.github/workflows/cmake_build.yml) | [![BuildAndTest](https://github.com/php1ic/nuclear-data-reader/workflows/BuildAndTest/badge.svg)](https://github.com/php1ic/nuclear-data-reader/actions) |    gcc9, clang10 |


## Introduction

The Nuclear Data reader is aimed at parsing and readeing the various files that have been published related to atomic nuclear data.
The format of the files is unique so takes some effort to parse.

No guarantee is supplied with regards to the accuracy of the data presented.
Estimated values are included, please always refer to the original sources.
All data should, however, be accurate.

Suggestions/improvements are encouraged, either create an [issue](https://github.com/php1ic/nuclear-data-reader/issues) or implement the change and send a pull request via [github](https://github.com/php1ic/nuclear-data-reader).

The code can read data from the following published papers:
- The Atomic Mass Evaluation
  - [2003](http://www.sciencedirect.com/science/article/pii/S0375947403018086) - [DOI](http://dx.doi.org/10.1016/j.nuclphysa.2003.11.002)
  - [2012](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract2709.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/36/12/002)
  - [2016](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract8344.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/41/3/030002)
  - [2020](https://iopscience.iop.org/article/10.1088/1674-1137/abddb0) - [DOI](https://doi.org/10.1088/1674-1137/abddb0)
- The NUBASE evaluation of nuclear and decay properties
  - [2003](http://www.sciencedirect.com/science/article/pii/S0375947403018074) - [DOI](http://dx.doi.org/10.1016/j.nuclphysa.2003.11.001)
  - [2012](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract2725.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/36/12/001)
  - [2016](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract8343.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/41/3/030001)
  - [2020](https://iopscience.iop.org/article/10.1088/1674-1137/abddae) - [DOI](https://doi.org/10.1088/1674-1137/abddae)

The data contained within is freely available as ASCII files, all of which are included with the project for ease.
The newer tables are available on the [AMDC](https://www-nds.iaea.org/amdc/) website, and the [historic](http://amdc.in2p3.fr/) page can be used to find older ones.


## Aquiring

Simplest way to aquire the repo is to clone it.
The project makes use of [fmt](https://github.com/fmtlib/fmt) and [Catch2](https://github.com/catchorg/Catch2) via submodules so they need to be initialised.
The below is enough to get you started.

```
$ git clone https://github.com/php1ic/nuclear-data-reader.git
$ cd nuclear-data-reader
$ git submodule update --init --recursive
```


## Building

The project is built using [cmake](https://cmake.org/) and instructions on building can be found in the file [INSTALL.md](INSTALL.md).

The files, within *./data/*, should **NOT** be altered.
Although they are all freely available from the above links if they are modified.


## Versioning
There is an attempt to follow [SemVer](http://semver.org/) for versioning numbers.
For the currently available versions, see the [tags on this repository](https://github.com/php1ic/nuclear-data-reader/tags).


## License
The code may be freely copied, distributed and altered under the terms of the [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.en.html) license.
Please read the file [COPYING](COPYING), which should be located at the root of the project, for full details.
