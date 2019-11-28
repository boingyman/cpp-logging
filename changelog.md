## Changelog

#### Planned

* Implimented compile-time logging level feature.
  * Removed all run-time logging level features, including:
    * `bta::logging::set_log_level(int)`
    * `bta::logging::get_log_level()`
    * `bta::logging::basic_log._loggingLevel`
    * `bta::logging::logging_internal::logger._loggingLevel`
    * Friend method `bta::logging::set_log_level(int)` in class `bta::logging::logging_internal::logger`
    * Friend method `bta::logging::get_log_level()` in class `bta::logging::logging_internal::logger`
  * Modified all features which relied on run-time logging level features to now use compile-time equivalents.

#### v1.0.0 [2018-12-29]

* Initial release
