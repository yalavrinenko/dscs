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
  explicit logger_sharedfs_error(std::string const &error): error(error){}
  const char *what() const noexcept override { return error.c_str(); }

private:
  std::string error;
};

class logger_entry;

class logger_factory {
public:
  explicit logger_factory(std::filesystem::path const &ramfs_path): ramfs_path(ramfs_path){};

  std::shared_ptr<logger_entry> create_logger(std::string const &name);

  void flush_loggers();

protected:
  struct entry_info{
    explicit entry_info(std::string const &name, size_t size = 1024 * 10);

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
  explicit logger_entry(std::filesystem::path path): linked_path_(std::move(path)){}

  template <class TArg>
  void log(TArg const &arg){
    log_impl(arg, "\n");
  }

  template <class TArg, class ... TArgs>
  void log(TArg const &arg1, TArgs ... args){
    log_impl(arg1, args..., "\n");
  }

  auto const& linked_path() const {
    return linked_path_;
  }

  ~logger_entry(){
    std::filesystem::remove(linked_path());
  }
protected:
  template <class TArg>
  void log_impl(TArg const &arg){
    buffer << arg << " ";
  }

  template <class TArg, class ... TArgs>
  void log_impl(TArg const &arg1, TArgs ... args){
    log_impl(arg1);
    log_impl(args...);
  }

  void clear(){
    buffer.clear();
    buffer.str(std::string{});
  }

  auto& get_buffer() const {return buffer;  }

  std::ostringstream buffer;

  std::filesystem::path const linked_path_;

  friend logger_factory;
};

using plogger = std::shared_ptr<logger_entry>;

#endif // DSCS_LOGGER_FACTORY_HPP
