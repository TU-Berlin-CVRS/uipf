Contributing
============

There are some rules to follow which are explained in the following.
Following these will raise the chance of your changes to get accepted quickly.

1. All new features and bug fixes should have an associated issue to provide a single point of
   reference for discussion and documentation. Take a few minutes to look through the existing
   issue list for one that matches the contribution you intend to make.
   If you find one already on the issue list, then please leave a comment on that issue indicating you intend to work on that item.
   If you do not find an existing issue matching what you intend to work on, please open a new issue or create a pull request directly if it is straightforward fix.

2. Create a new branch for submitting pull requests, do not work on your master branch.
   Here is a good documentation on how it works: https://guides.github.com/introduction/flow/

3. When making changes, please add a short notice to the [CHANGELOG](CHANGELOG.md) file.
   The purpose of this file is to provide an overview about what has changed between different versions.

4. When you send a pull request, you provide your contribution under the same [BSD license](LICENSE) that is used
   by the project.

Thank you for your contribution!

Coding Guidelines and code style
--------------------------------

The exact code formatting is not super important. What is important that code is
in a similar shape to ensure good readability.
We try to follow the following guidelines for code formatting:
http://geosoft.no/development/cppstyle.html

Naming of files:

- Create two files per class, one `ClassName.hpp` for the header file and a `ClassName.cpp` for the class implementation ([why?](http://stackoverflow.com/questions/152555/h-or-hpp-for-your-class-definitions)).
- Exception: Qt sometimes forces some header files to end with `.h`
- Classes are named in CamelCase, class files should have the same case as the class name.

