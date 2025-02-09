#include "mod/MyMod.h"
#include "ll/api/mod/RegisterHelper.h"

#include <fmt/format.h>
#include <functional>

#include <ll/api/command/Command.h>
#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/event/player/PlayerJoinEvent.h>

#include <ll/api/service/Bedrock.h>
#include <mc/world/actor/ActorType.h>
#include <mc/world/actor/player/Player.h>
#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/server/commands/CommandPermissionLevel.h>


namespace my_mod {

MyMod& MyMod::getInstance() {
    static MyMod instance;
    return instance;
}

bool MyMod::load() {
    getSelf().getLogger().debug("Loading...");
    // Code for loading the mod goes here.
    return true;
}

bool MyMod::enable() {
    getSelf().getLogger().info("ping插件已运行...");
    

    // Register commands.
    auto commandRegistry = ll::service::getCommandRegistry();
    if (!commandRegistry) {
        throw std::runtime_error("failed to get command registry");
    }

    const auto& logger = MyMod::getInstance().getSelf().getLogger();
    
    logger.debug("debug 日志命令已注册");

    auto& command = ll::command::CommandRegistrar::getInstance().getOrCreateCommand("suicide", "Commits suicide.", CommandPermissionLevel::Any);

    command.overload().execute(
        [&](CommandOrigin const& origin, CommandOutput& output)
        {
            auto* entity = origin.getEntity();
            if (entity == nullptr || !entity->isType(ActorType::Player)) {
                output.error("Only players can commit suicide");
                return;
            }
    
            auto* player = static_cast<Player*>(entity); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            player->kill();
    
            logger.info("{} killed themselves", player->getRealName());
        });

    auto& command2 = ll::command::CommandRegistrar::getInstance().getOrCreateCommand("ip", "获取IP地址", CommandPermissionLevel::Any);

    command2.overload().execute(
        [&](CommandOrigin const& origin, CommandOutput& output)
        {
            auto* entity = origin.getEntity();
            if (entity == nullptr || !entity->isType(ActorType::Player)) {
                output.error("Only players can commit suicide");
                return;
            }
    
            auto* player = static_cast<Player*>(entity); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            player->sendMessage("你的地址是: "+player->getIPAndPort());

            logger.info("{} 地址: ", player->getIPAndPort());
        });
    return true;
}

bool MyMod::disable() {
    getSelf().getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
    return true;
}

} // namespace my_mod

LL_REGISTER_MOD(my_mod::MyMod, my_mod::MyMod::getInstance());
