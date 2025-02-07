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

| OS             | Config                                       |                                                                                                                                                                            Build Status | Compiler Version |
|----------------|----------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|-----------------:|
| Linux          | [linux.yml](.github/workflows/linux.yml)     |       [![Linux Builds](https://github.com/php1ic/nuclear-data-reader/actions/workflows/linux.yml/badge.svg)](https://github.com/php1ic/nuclear-data-reader/actions/workflows/linux.yml) |   gcc13, clang18 |
| Windows (MSVC) | [windows.yml](.github/workflows/windows.yml) | [![Windows Builds](https://github.com/php1ic/nuclear-data-reader/actions/workflows/windows.yml/badge.svg)](https://github.com/php1ic/nuclear-data-reader/actions/workflows/windows.yml) |        MSVC-2022 |

## Introduction

The Nuclear Data Reader is aimed at parsing and reading the various files that have been published related to atomic nuclear data.
The format of the files is unique, and in constant flux, so takes some effort to parse.
This project takes away the need to keep worry about those things and creates [json](https://www.json.org/) formatted files.

No guarantee is supplied with regards to the accuracy of the data presented.
Estimated values are included, please always refer to the original sources.
All data should, however, be accurate.

Suggestions/improvements are encouraged, either create an [issue](https://github.com/php1ic/nuclear-data-reader/issues) or implement the change and send a pull request via [github](https://github.com/php1ic/nuclear-data-reader).

The code can read data from the following published papers:

| Year | Atomic Mass Evaluation                                                                                                                                                                                           | NUBASE                                                                                                                                  |
|------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------|
| 1971 | [URL](https://link.springer.com/chapter/10.1007/978-1-4684-7876-1_30) - [DOI](https://doi.org/10.1007/978-1-4684-7876-1_30) (Investigating if data files exists)                                                 | :x:                                                                                                                                     |
| 1977 | [URL](https://www.sciencedirect.com/science/article/abs/pii/0092640X77900043) - [DOI](https://doi.org/10.1016/0092-640X(77)90004-3) (Investigating if data files exists)                                         | :x:                                                                                                                                     |
| 1983 | [URL](https://www.sciencedirect.com/science/article/abs/pii/0375947485902830) - [DOI](https://doi.org/10.1016/0375-9474(85)90283-0)                                                                              | :x:                                                                                                                                     |
| 1993 | [URL](https://www.sciencedirect.com/science/article/abs/pii/037594749390024R) - [DOI](https://doi.org/10.1016/0375-9474(93)90024-R)                                                                              | :x:                                                                                                                                     |
| 1995 | [URL](https://www.sciencedirect.com/science/article/abs/pii/0375947495004459) - [DOI](https://doi.org/10.1016/0375-9474(95)00445-9)                                                                              | :x:                                                                                                                                     |
| 1997 | No AME, but the 1995AME matches the 1997NUBASE.<br>See section 4, "The tables" on P31 of [these proceedings](https://www.google.co.uk/books/edition/Atomic_Physics_at_Accelerators_Mass_Spec/3AbsCAAAQBAJ?hl=en) | [URL](https://www.sciencedirect.com/science/article/abs/pii/S037594749700482X) - [DOI](https://doi.org/10.1016/S0375-9474(97)00482-X)   |
| 2003 | [URL](http://www.sciencedirect.com/science/article/pii/S0375947403018086) - [DOI](http://dx.doi.org/10.1016/j.nuclphysa.2003.11.002)                                                                             | [URL](https://www.sciencedirect.com/science/article/abs/pii/S037594749700482X) - [DOI](https://doi.org/10.1016/S0375-9474(97)00482-X)   |
| 2012 | [URL](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract2709.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/36/12/002)                                                                            | [URL](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract2725.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/36/12/001)   |
| 2016 | [URL](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract8344.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/41/3/030002)                                                                          | [URL](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract8343.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/41/3/030001) |
| 2020 | [URL](https://iopscience.iop.org/article/10.1088/1674-1137/abddb0) - [DOI](https://doi.org/10.1088/1674-1137/abddb0)                                                                                             | [URL](https://iopscience.iop.org/article/10.1088/1674-1137/abddae) - [DOI](https://doi.org/10.1088/1674-1137/abddae)                    |

The data contained within is freely available as ASCII files, all of which are included with the project for ease.
The newer tables are available on the [AMDC](https://www-nds.iaea.org/amdc/) website, and the [historic](http://amdc.in2p3.fr/) page can be used to find older ones.


## Acquiring

Simplest way to acquire the repo is to clone it.
The project makes use of [fmt](https://github.com/fmtlib/fmt) and [Catch2](https://github.com/catchorg/Catch2) via submodules so they need to be initialised.
The below is enough to get you started.

```
$ git clone --recurse-submodules https://github.com/php1ic/nuclear-data-reader.git
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
