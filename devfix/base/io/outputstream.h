//
// Created by core on 12/27/19.
//

#pragma once

#include <cstdint>

namespace devfix::base::io
{

/**
 * \brief Superclass of all classes representing an output stream of bytes.
 *
 * An output stream accepts output bytes and sends them to some sink.
 * Applications that need to define a subclass of OutputStream must always provide a method that writes one byte of output.
 */
struct outputstream
{
  virtual ~outputstream() = default;

  /**
   * \brief Writes len bytes from the specified buffer to this output stream.
   *
   * Element b[0] is the first byte written and b[len-1] is the last byte written by this operation.
   *
   * @param buf the data.
   * @param len the number of bytes to write.
   */
  virtual void write(const void * buf, std::size_t len) = 0;

  /**
   * \brief Flushes this <i>outputstream</i> and forces any buffered output bytes to be written out.
   *
   * The general contract of flush is that calling it is an indication that, if any bytes previously written have been buffered by the implementation of the output stream, such bytes should immediately be written to their intended destination.
   *
   * If the intended destination of this stream is an abstraction provided by the underlying operating system, for example a file, then flushing the stream guarantees only that bytes previously written to the stream are passed to the operating system for writing; it does not guarantee that they are actually written to a physical device such as a disk drive.
   */
  virtual void flush() = 0;

  /**
   * \brief Closes this <i>outputstream</i> and releases any system resources associated with this stream.
   *
   * The general contract of close is that it closes the output stream.
   * A closed stream cannot perform output operations and cannot be reopened.
   */
  virtual void close() = 0;

  /**
   * \brief Returns if the <i>outputstream</i> is closed or available for further calls of output operations.
   *
   * @return true if the <i>outputstream</i> got previously closed.
   */
  [[nodiscard]] virtual bool is_closed() = 0;
};

} // namespace devfix::base::io