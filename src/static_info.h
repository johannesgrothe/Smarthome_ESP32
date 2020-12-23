
/**
 * Gets the name of the git branch of the currently running software
 * @return the git branch name
 */
std::string getSoftwareGitBranch() {
  #ifdef SW_GIT_BRANCH
  std::stringstream sstr;
  sstr << SW_GIT_BRANCH;
  return sstr.str();
  #else
  return "<unknown>";
  #endif
}

/**
 * Gets the git commit hash of the currently running software
 * @return
 */
std::string getSoftwareGitCommit() {
  #ifdef SW_GIT_COMMIT
  std::stringstream sstr;
  sstr << SW_GIT_COMMIT;
  return sstr.str();
  #else
  return "<unknown>";
  #endif
}

/**
 * Gets the date the currently running software was flashed
 *
 * Format: yyyy-mm-dd
 * @return The date the current software was flashed
 */
std::string getSoftwareFlashDate() {
  #ifdef SW_FLASH_TIME
  std::stringstream sstr;
  sstr << SW_FLASH_TIME;
  return sstr.str();
  #else
  return "<unknown>";
  #endif
}
