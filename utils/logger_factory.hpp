//
// Created by yalavrinenko on 29.06.19.
//

#ifndef DSCS_LOGGER_FACTORY_HPP
#define DSCS_LOGGER_FACTORY_HPP

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>

class logger_sharedfs_error: std::exception{
public:
  explicit logger_sharedfs_error(std::string error): error(std::move(error)){}
  [[nodiscard]] const char *what() const noexcept override { return error.c_str(); }

private:
  std::string error;
};

class logger_entry;

class logger_factory {
public:
  explicit logger_factory(std::filesystem::path ramfs_path): ramfs_path(std::move(ramfs_path)){};

  std::shared_ptr<logger_entry> create_logger(std::string const &name, size_t size=1024*10);

  void flush_loggers();

protected:
  struct entry_info{
    explicit entry_info(std::string const &name, logger_factory* factory, size_t size = 1024 * 10);

    int create_shared_file(std::string const &name, size_t size);

    void flush();

    ~entry_info();

    std::shared_ptr<logger_entry> user_entry;
    uint8_t* memptr = nullptr;
    size_t memsize;
    int file_descr = -1;
  };

  std::vector<std::unique_ptr<entry_info>> loggers;
  std::filesystem::path ramfs_path;
};

class logger_entry{
public:
  explicit logger_entry(std::filesystem::path path, logger_factory* factory):
    factory_{factory},
    linked_path_(std::move(path)){}

  template <class ... TArgs>
  void log(TArgs ... args){
    std::string shift(log_level_, '\t');
    log_impl(shift);
    (log_impl(args), ...);
    log_impl("\n");
  }

  auto const& linked_path() const {
    return linked_path_;
  }

  void up_level() { log_level_ += 1; }

  void down_level() { if (log_level_ != 0) log_level_ -= 1; }

  logger_factory* factory() { return factory_; }

  ~logger_entry(){
    std::filesystem::remove(linked_path());
  }

protected:
  template <class TArg>
  void log_impl(TArg const &arg){
    buffer << arg << " ";
  }

  void clear(){
    buffer.clear();
    buffer.str(std::string{});
  }

  auto& get_buffer() const {return buffer;  }

  std::ostringstream buffer;

  std::filesystem::path const linked_path_;

  size_t log_level_{0};

  logger_factory* factory_ = nullptr;

  friend logger_factory;
};

using plogger = std::shared_ptr<logger_entry>;

#endif // DSCS_LOGGER_FACTORY_HPP
