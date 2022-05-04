End-of-Life
========

This project will reach end-of-life on September 1st, 2022. On this date:

1. This repository will be [archived](https://docs.github.com/en/repositories/archiving-a-github-repository/archiving-repositories), making it read-only.
2. The Google Group will be locked, but will remain publicly searchable.
3. The project homepage, this README, and group will be updated to indicate that the project is end-of-life.

We will consider bug fixes from the community or support requests from existing customers up until this date.

Please make appropriate plans if you are using this library in production, e.g.:

* Dedicate personnel to maintaining your own internal copy of the library.
* Consider a commercial library such as the one offered by [Step Function I/O](https://stepfunc.io/products/libraries/dnp3/).

You can read about this decision in these blog posts:

* [OpenDNP3 Retrospective](https://stepfunc.io/blog/opendnp3-retrospective/)
* [DNP3 1.0.0 (Rust)](https://stepfunc.io/blog/dnp3_1_0_0/)


Overview
========

Opendnp3 is a portable, scalable, and rigorously tested implementation 
of the [DNP3](https//www.dnp.org) protocol stack written in C++11. The library 
is designed for high-performance applications like many concurrent TCP
sessions or huge device simulations. It also embeds with a small footprint on Linux.

Build status
============

| Branch       | Build | Code coverage | Quality |
| ------------ | ----- | ------------- | ------- |
| release-2.x  | [![CI 2.x](https://github.com/dnp3/opendnp3/workflows/CI/badge.svg?branch=release-2.x)](https://github.com/dnp3/opendnp3/actions?query=branch%3Arelease-2.x) | [![Codecov](https://codecov.io/gh/dnp3/opendnp3/branch/release-2.x/graph/badge.svg)](https://codecov.io/gh/dnp3/opendnp3/branch/release-2.x) | - |
| develop      | [![CI 2.x](https://github.com/dnp3/opendnp3/workflows/CI/badge.svg?branch=develop)](https://github.com/dnp3/opendnp3/actions?query=branch%3Adevelop) | [![Codecov](https://codecov.io/gh/dnp3/opendnp3/branch/develop/graph/badge.svg)](https://codecov.io/gh/dnp3/opendnp3/branch/develop) | [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/dnp3/opendnp3.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/dnp3/opendnp3/context:cpp) |

Documentation
=============

The documentation can be found on the [project homepage](http://dnp3.github.io/#documentation).

If you want to help contribute to the official guide its in [this repo](https://github.com/dnp3/opendnp3-guide).

License
=============

Licensed under the terms of the [Apache 2.0 License](http://www.apache.org/licenses/LICENSE-2.0.html).

Copyright (c) 2010, 2011 Green Energy Corp

Copyright (c) 2013 - 2020 Step Function I/O LLC

Copyright (c) 2020 - 2022 Step Function I/O LLC

Copyright (c) 2010 - 2022 various contributors
