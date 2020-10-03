//
// Created by aaron on 2020-09-30.
//

#ifndef P1_HELPER_HPP
#define P1_HELPER_HPP


#define MAX_COMMAND_LENGTH 5000

namespace cis427 {

    typedef char* array;

    /**
     * Converts a string to a char array to use as a buffer in the socket
     * @param str string to be put into buffer
     * @return buffer from string, extra spaces will be filled with \0 000
     */
    inline const char * to_buff(const std::string &str) {
        return str.c_str();
    }

    /**
     * Unused so far, just put this here so I wouldnt forget how to convert between the two
     * @param buff
     * @return a string from the buffer
     */
    inline std::string buff_to_string(const char * buff){
        return std::string(buff);
    }

    /**
     * This is called in the server before the recv is called again. If you dont
     * clear the buffer then if the last message was longer than the current then
     * you will have extra characters in your buffer
     * @param buff buffer to be filled with \0 000
     */
    inline void clear_buff(char * buff){
        for(int i=0;i<MAX_COMMAND_LENGTH;i++){
            buff[i] = '\0';
        }
    }

    /**
     * @brief this struct is just an object that contains a response code and a message
     * Its used for sending info to clients. It also contains helpful constructors to make
     * conversions easier in the future
     */
    struct Response{
        Response(){};
        Response(const int& icode, const std::string& ibuff){
            code = icode;
            buff = ibuff;
        }
        Response(const char* ibuff){
            std::string buffstr = std::string(ibuff);
            if(!buffstr.empty()) {
                code = std::stoi(buffstr.substr(0, 3));
                buff = buffstr.substr(4);
            }
        }

        const char * to_buff(){
            std::string bstr = std::to_string(code) + ' ' + buff;
            char * ret = new char[MAX_COMMAND_LENGTH];
            for(int i=0;i<bstr.size();i++){
                ret[i] = bstr.at(i);
            }
            return ret;
        }

        int code;
        std::string buff;
    };

}

#endif