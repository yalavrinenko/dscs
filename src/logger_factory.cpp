//
// Created by yalavrinenko on 29.06.19.
//

#include "logger_factory.hpp"
using namespace std::string_literals;

std::shared_ptr<logger_entry>
logger_factory::create_logger(std::string const &name, size_t size) {
  loggers.emplace_back(std::make_unique<entry_info>(ramfs_path / name, size));
  return loggers.back()->user_entry;
}

void logger_factory::flush_loggers() {
  for (auto &logger : loggers)
    logger->flush();
}

logger_factory::entry_info::entry_info(std::string const &name, size_t size): memsize(size) {
  user_entry = std::make_shared<logger_entry>(name);
  file_descr = create_shared_file(name, memsize);
  memptr = static_cast<uint8_t *>(
      mmap(nullptr, memsize, PROT_READ | PROT_WRITE, MAP_SHARED, file_descr, 0)
  );

  if (memptr == nullptr)
    throw logger_sharedfs_error("mmap error. "s + std::to_string(errno));
}

int logger_factory::entry_info::create_shared_file(std::string const &name,
                                                   size_t size) {
  auto fs_descr = open(name.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRWXO | S_IRWXU | S_IRWXG);
  if (fs_descr < 0)
    throw logger_sharedfs_error("Open file error. "s + std::to_string(errno));

  auto off = ftruncate(fs_descr, size);
  if (off == -1)
    throw logger_sharedfs_error("Resize file error. "s + std::to_string(errno));

  return fs_descr;
}

void logger_factory::entry_info::flush() {
  std::string str = this->user_entry->get_buffer().str() + "EOF";
  std::copy(str.begin(), str.end(), this->memptr);
  this->user_entry->clear();
  msync(memptr, memsize, MS_SYNC);
}

logger_factory::entry_info::~entry_info() {
  flush();
  munmap(this->memptr, this->memsize);
  close(this->file_descr);
}
