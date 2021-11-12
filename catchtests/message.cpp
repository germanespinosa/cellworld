#include<catch.h>
#include<cell_world.h>

using namespace cell_world;
using namespace std;

TEST_CASE("basic message"){
    Message message("title","body");
    CHECK(message.to_json() == "{\"title\":\"title\",\"body\":\"body\"}");
}

TEST_CASE("json body"){
    Message message("title",Message("title","body"));
    CHECK(message.to_json() == "{\"title\":\"title\",\"body\":\"{\\\"title\\\":\\\"title\\\",\\\"body\\\":\\\"body\\\"}\"}");
}

TEST_CASE("json set_body"){
    Message message("title");
    message.set_body(Message("title","body"));
    CHECK(message.to_json() == "{\"title\":\"title\",\"body\":\"{\\\"title\\\":\\\"title\\\",\\\"body\\\":\\\"body\\\"}\"}");
}

TEST_CASE("json get_body"){
    Message message("title",Message("title","body"));
    CHECK(message.get_body<Message>().to_json() == Message("title","body").to_json());
}