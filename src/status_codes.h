#pragma once

enum class WriteGadgetStatus {
  WritingOK,
  MaxGadgetCountReached,
  NameAlreadyInUse,
  PortNotConfigured,
  PortAlreadyInUse,
  MissingEEPROMSpace,
  ErrorWritingContent,
  ErrorSavingMemoryEnd,
  GadgetTypeError0,
  GadgetTypeErrorUnknown,
  FaultyConfigJSON,
  FaultyCodeConfig,
  DeletionFailed
};

std::string writeGadgetStatusToString(WriteGadgetStatus code) {
  switch (code) {
    case WriteGadgetStatus::WritingOK:
      return "writing ok";
    case WriteGadgetStatus::MaxGadgetCountReached:
      return "maximum amount of gadgets reached";
    case WriteGadgetStatus::NameAlreadyInUse:
      return "gadget name is already in use";
    case WriteGadgetStatus::PortNotConfigured:
      return "gadget tries to occupy a port that is not configured on the system";
    case WriteGadgetStatus::PortAlreadyInUse:
      return "gadget tries to occupy a port that is already in use";
    case WriteGadgetStatus::MissingEEPROMSpace:
      return "missing space in eeprom";
    case WriteGadgetStatus::ErrorWritingContent:
      return "error writing content";
    case WriteGadgetStatus::ErrorSavingMemoryEnd:
      return "error writing memory end";
    case WriteGadgetStatus::GadgetTypeError0:
      return "not saving gadget with error-type 0";
    case WriteGadgetStatus::GadgetTypeErrorUnknown:
      return "not saving gadget with unknown type";
    case WriteGadgetStatus::FaultyConfigJSON:
      return "received faulty config json";
    case WriteGadgetStatus::FaultyCodeConfig:
      return "received faulty code config";
    case WriteGadgetStatus::DeletionFailed:
      return "failed to delete the gadget for rewriting";
    default:
      return "unknown status code";
  }
}