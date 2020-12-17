#pragma once

#include <ArduinoJson.h>
#include <cstring>
#include <utility>
#include <memory>

#include "../console_logger.h"

class Request {
private:
  // basic information the request is carrying
  std::string path_;
  int session_id_;
  std::string sender_;
  std::string receiver_;
  DynamicJsonDocument payload_;

  // respond to (incomming) request
  bool needs_response_;
  bool can_respond_;
  std::function<void(std::shared_ptr<Request>)> send_answer_;

  // wait for answer to request
  bool await_response_;
  std::shared_ptr<Request> response_;

public:
  /**
   * Constructor for the Request if there should ba an option to send an answer.
   * This constructor should only be used by the receiving gadgets to attach their response-methods
   * @param req_path the path the request should be send to
   * @param session_id the session id the request should use to identify itself
   * @param sender the sender of the request
   * @param receiver the receiver of the request
   * @param payload the payload that should be send within the request
   * @param await_answer whether the sender awaits an answer or not. Defaults to false.
   */
  Request(std::string req_path,
          int session_id,
          std::string sender,
          std::string receiver,
          DynamicJsonDocument payload,
          bool await_answer=false);

  /**
   * Constructor for the Request if there should ba an option to send an answer.
   * This constructor should only be used by the receiving gadgets to attach their response-methods
   * @param req_path the path the request should be send to
   * @param session_id the session id the request should use to identify itself
   * @param sender the sender of the request
   * @param receiver the receiver of the request
   * @param payload the payload that should be send within the request
   * @param answer_method the method that the internal answer()-methods use to respond to an incoming request
   */
  Request(std::string req_path,
          int session_id,
          std::string sender,
          std::string receiver,
          DynamicJsonDocument payload,
          std::function<void(std::shared_ptr<Request> request)> answer_method);

  /**
   * Destructor
   */
  virtual ~Request();

  /**
   * Method to access the path of the request
   * @return the path
   */
  std::string getPath() const;

  /**
   * Method to access the session id of the request
   * @return the id
   */
  int getID() const;

  /**
   * Method to access the sender of the request
   * @return the sender
   */
  std::string getSender() const;

  /**
   * Method to check if the request has an receiver
   * @return whether the request has an receiver
   */
  bool hasReceiver() const;

  /**
   * Method to access the receiver of the request.
   * Use hasReceiver() to check if there is one in the first place.
   * Method returns 'null' if there is none.
   * @return the receiver
   */
  std::string getReceiver() const;

  /**
   * Changes the receiver in the request to the new value.
   * @param receiver the new receiver for the Request
   */
  void updateReceiver(std::string receiver);

  /**
   * Method to access the payload of the request
   * @return the payload
   */
  DynamicJsonDocument getPayload() const;

  /**
   * Method to access the whole body of the request.
   * You may be looking for the payload only.
   * @return the body as a json string
   */
  std::string getBody() const;

  /**
   * Responds to this request with the original path and a simple ack body
   * @param ack whether the action the original request triggered was successful
   * @return whether responding was successful
   */
  bool respond(bool ack);

  /**
   * Responds to this request with the given path and a simple ack body
   * @param ack whether the action the original request triggered was successful
   * @param path the path the response should be sent to
   * @return whether responding was successful
   */
  bool respond(bool ack, const std::string& path);

  /**
   * Responds to this request with the given path and a simple ack body
   * @param ack whether the action the original request triggered was successful
   * @param status_msg the status message explaining the ack
   * @param path the path the response should be sent to
   * @return whether responding was successful
   */
  bool respond(bool ack, const std::string& status_msg, const std::string& path);

  /**
   * Responds to this request with the original path and payload.
   * @param payload the payload that should be sent
   * @return whether responding was successful
   */
  bool respond(const DynamicJsonDocument& payload);

  /**
   * Responds to this request with the given path and payload.
   * @param path the path the response should be sent to
   * @param payload the payload that should be sent
   * @return whether responding was successful
   */
  bool respond(const std::string& path, const DynamicJsonDocument& payload);

  /**
   * Discards the 'needs response'-flag without actually sending one
   */
  void dontRespond();

  /**
   * Checks if the request has a ack-status and an status message.
   * Returns them if there are any, returns false if there are none.
   * @return {ack-status, stauts-message}
   */
  std::tuple<bool, std::string> getAck();
};