#include<catch.h>
#include<cell_world.h>

using namespace cell_world;
using namespace std;

TEST_CASE("basic message"){
    Message message("header","body");
    CHECK(message.to_json() == "{\"header\":\"header\",\"body\":\"body\"}");
}

TEST_CASE("json body"){
    Message message("header",Message("header","body"));
    CHECK(message.to_json() == "{\"header\":\"header\",\"body\":\"{\\\"header\\\":\\\"header\\\",\\\"body\\\":\\\"body\\\"}\"}");
}

TEST_CASE("json set_body"){
    Message message("header");
    message.set_body(Message("header","body"));
    CHECK(message.to_json() == "{\"header\":\"header\",\"body\":\"{\\\"header\\\":\\\"header\\\",\\\"body\\\":\\\"body\\\"}\"}");
}

TEST_CASE("json get_body"){
    Message message("header",Message("header","body"));
    CHECK(message.get_body<Message>().to_json() == Message("header","body").to_json());
}