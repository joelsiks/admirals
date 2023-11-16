#pragma once

namespace admirals {
namespace net {

template <typename T> struct MessageHeader {
    T id{};
    uint32_t size = 0;
};

template <typename T> struct Message {
    MessageHeader<T> header{};
    std::vector<uint8_t> body;

    // Returns the size of the message body in bytes
    size_t Size() const { return body.size(); }

    // Overload for the << operator for adding data to the message buffer
    template <typename DataType>
    friend Message<T> &operator<<(Message<T> &msg, const DataType &data) {
        // Check that the data type is trivially copyable
        static_assert(std::is_standard_layout<DataType>::value,
                      "Data is too complex to be pushed into vector");

        // Resize the message and copy the new data into the end of the buffer
        size_t prev_size = msg.body.size();
        msg.body.resize(msg.body.size() + sizeof(DataType));
        std::memcpy(msg.body.data() + prev_size, &data, sizeof(DataType));
        msg.header.size = msg.Size();

        return msg;
    }

    // Overload for the >> operator for pulling data from the message buffer
    template <typename DataType>
    friend Message<T> &operator>>(Message<T> &msg, DataType &data) {
        // Check that the data type is trivially copyable
        static_assert(std::is_standard_layout<DataType>::value,
                      "Data is too complex to be pulled from vector");

        size_t new_size = msg.body.size() - sizeof(DataType);
        std::memcpy(&data, msg.body.data() + new_size, sizeof(DataType));
        msg.body.resize(new_size);
        msg.header.size = msg.Size();

        // Return the target message so it can be chained
        return msg;
    }
};

// Forward declaration of Connection class to use in OwnedMessage
template <typename T> class Connection;

template <typename T> struct OwnedMessage {
    std::shared_ptr<Connection<T>> remote = nullptr;
    Message<T> message;
};

} // namespace net
} // namespace admirals
