#pragma once

#include "segments/core/connectors/FileInputConnector/FileInputConnector.h"
#include "segments/core/connectors/FileOutConnector/FileOutConnector.h"
#include "segments/shared/DataStructs.h"
#include "segments/shared/Types.h"
#include "segments/shared/Constants.h"

#include <stdexcept>


namespace Core {
    ProcessResponse processQuery(const std::string &inputFilename, std::string &outputDirname,
                                 FilePacket &inputFilePacket, const Query &query);
}
