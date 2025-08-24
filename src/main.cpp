#include <spdlog/spdlog.h>

#include "engine/core/GameApp.hpp"


int main()
{
    spdlog::set_level(spdlog::level::trace);
    engine::core::GameApp app;
    app.run();
    return 0;
}
