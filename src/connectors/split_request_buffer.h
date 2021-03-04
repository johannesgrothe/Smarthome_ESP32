class SplitRequestBuffer {
private:
  std::vector<std::string> data_buffer_;
  int last_index = 0;

public:
  const int session_id_;
  const int length_;

  SplitRequestBuffer(int session_id, int length);

  void addData(int index, std::string payload);

  Request * getRequest();
};