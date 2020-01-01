//
// Created by core on 12/27/19.
//

#pragma once

#include <cstdint>

namespace devfix::base::io
{

/**
 * @brief Superclass of all classes representing an input stream of bytes.
 *
 * Applications that need to define a subclass of InputStream must always provide a method that returns the next byte of input.
 */
struct inputstream
{

  /**
   * @brief Default virtual destructor.
   *
   * Needed for correct deletion of instances of a derived classes through a pointer to base class.
   */
  virtual ~inputstream() = default;

  /**
   * \brief Reads bytes from the input stream and stores them into the buffer.
   *
   * This method blocks until input data is available, end of file is detected, or another exception is thrown.
   *
   * If len is zero, then no bytes are read.
   * If no byte is available because the stream is at end of file, an exception is thrown.
   *
   * The first byte read is stored into element b[0], the next one into b[1], and so on.
   * If no exception was thrown, the number of bytes read is always equal to len.
   *
   * Subclasses are encouraged to provide a more efficient implementation of this method.
   *
   * @param buf the buffer into which the data is read.
   * @param len the maximum number of bytes to read.
   */
  virtual void read(void *buf, std::size_t len) = 0;

  /**
   * \brief Skips over and discards n bytes of data from this input stream.
   *
   * The skip method may, for a variety of reasons, end up skipping over some smaller number of bytes, possibly 0.
   * This may result from any of a number of conditions; reaching end of file before n bytes have been skipped is only one possibility.
   *
   * @param n the number of bytes to be skipped.
   */
  virtual void skip(std::size_t n) = 0;

  /**
   * \brief Returns an estimate of the number of bytes that can be read (or skipped over) from this input stream without blocking by the next invocation of a method for this input stream.
   *
   * A single read or skip of this many bytes will not block, but may read or skip fewer bytes.
   *
   * Note that while some implementations of <i>inputstream</i> will return the total number of bytes in the stream, many will not.
   * It is never correct to use the return value of this method to allocate a buffer intended to hold all data in this stream.
   *
   * A subclass' implementation of this method may choose to throw an IOException if this input stream has been closed by invoking the close() method.
   *
   * This method should be overridden by subclasses.
   *
   * @returns an estimate of the number of bytes that can be read (or skipped over) from this input stream without blocking or 0 when it reaches the end of the input stream.
   */
  [[nodiscard]] virtual std::size_t available() = 0;

  /**
   * \brief Closes this input stream and releases any system resources associated with the stream.
   *
   * A closed stream cannot perform input operations and cannot be reopened.
   */
  virtual void close() = 0;

  /**
   * \brief Returns if the <i>inputstream</i> is closed or available for further calls of input operations.
   *
   * @return true if the <i>inputstream</i> got previously closed.
   */
  [[nodiscard]] virtual bool is_closed() = 0;

};

} // namespace devfix::base::io