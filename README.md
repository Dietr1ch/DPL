DPL
===

DPL is a planning library heavily based on the SBPL written from scratch to
  learn and also te build a cleaner library.



##### Why another project?
SBPL may work fine, but I did not liked many details of the implementation
  that made the code hard to read (at least to me).

Some things that annoyed me enough to start from scratch:
  * The C++ code was heavily C-like, not using many features that help to
      write consise and clear code. I felt that developers had fear of using
      typedefs, constructors, references and streams only to name some.
  * Some C++11 features that make the code way more readable were not used,
      I don’t care if C++11 is too new or was not ready at the time, now
      it’s available and it makes C++ nicer.
  * The SBPL code seemed quite fragmented to me, as the available code upstream
      was not jointly developed by the planning community. During some time I
      got my hands into multiple projects that used SBPL as a base library for
      research, but the changes were never introduced back, so I had many
      SBPLish codebases that were not easily compatible.


Additional Resources
====================
  * [SBPL](http://www.sbpl.net/)
  * [SBPL on github](https://github.com/sbpl/sbpl)
