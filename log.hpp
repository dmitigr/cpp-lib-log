// -*- C++ -*-
//
// Copyright 2023 Dmitry Igrishin
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DMITIGR_LOG_LOG_HPP
#define DMITIGR_LOG_LOG_HPP

#include "../chrx/format.hpp"
#include "../fsx/filesystem.hpp"
#include "../os/pid.hpp"
#include "exceptions.hpp"

#include <atomic>
#include <fstream>
#include <iostream>

namespace dmitigr::log {

namespace detail {
inline std::ofstream log_file_stream;
} // namespace detail

/// @see clog().
inline std::atomic_bool is_clog_with_now;

/// Creating the file at `path` and dumping the PID into it.
inline void dump_pid(const std::filesystem::path& path)
{
  if (std::ofstream pf{path, std::ios_base::trunc})
    pf << os::pid() << std::endl;
  else
    throw Exception{"cannot open PID file " + path.string()};
}

/// Redirecting `std::clog` to file at `path`.
inline void redirect_clog(const std::filesystem::path& path,
  const std::ios_base::openmode openmode = std::ios_base::app | std::ios_base::ate)
{
  if ( (detail::log_file_stream = std::ofstream{path, openmode}))
    std::clog.rdbuf(detail::log_file_stream.rdbuf());
  else
    throw Exception{"cannot open log file " + path.string()};
}

/// @returns std::clog after writing now string to it first.
inline std::ostream& clog_now() noexcept
{
  return std::clog << dmitigr::chrx::now() << ": ";
}

/**
 * @returns The stream for starting log entry.
 *
 * @par Effects
 * If `is_clog_with_now` then puts the result of clog_now() to the `std::clog`.
 */
inline std::ostream& clog() noexcept
{
  return is_clog_with_now ? clog_now() : std::clog;
}

} // namespace dmitigr::log

#endif  // DMITIGR_LOG_LOG_HPP
