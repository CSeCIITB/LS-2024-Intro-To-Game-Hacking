#include <set>
#include <map>
#include <string>
#include <vector>
#include "iactor.h"
#include "projectile.h"
#include "iplayer.h"
#include "aizone.h"
#include "player.h"


class World {
  protected:
    class std::set<ActorRef<IPlayer>, std::less<ActorRef<IPlayer> >, std::allocator<ActorRef<IPlayer> > > m_players;
    class std::set<ActorRef<IActor>, std::less<ActorRef<IActor> >, std::allocator<ActorRef<IActor> > > m_actors;
    class std::map<unsigned int, ActorRef<IActor>, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, ActorRef<IActor> > > > m_actorsById;
    class ILocalPlayer *m_localPlayer;
    uint32_t m_nextId;
    class std::map<std::string, AIZone*, std::less<std::string>, std::allocator<std::pair<std::string const, AIZone*> > > m_aiZones;

    void AddActorToWorld(class Actor *);
    void AddActorToWorldWithId(uint32_t, class Actor *);
    void SendEventToAllPlayers(const class WriteStream &);
    void SendEventToAllPlayersExcept(class Player *, 
        const class WriteStream &);
  public:
    World();
    virtual ~World();
    virtual void Tick(float);
    virtual bool HasLocalPlayer();
    class ILocalPlayer * GetLocalPlayer();
    virtual bool IsAuthority();
    virtual void AddLocalPlayer(class Player *, class ILocalPlayer *);
    virtual void AddRemotePlayer(class Player *);
    virtual void AddRemotePlayerWithId(uint32_t, class Player *);
    virtual void RemovePlayer(class Player *);
    virtual void Use(class Player *, class Actor *);
    virtual void Activate(class Player *, class IItem *);
    virtual void Reload(class Player *);
    virtual void Jump(bool);
    virtual void Sprint(bool);
    virtual void FireRequest(bool);
    virtual void TransitionToNPCState(class Player *, const std::string &);
    virtual void BuyItem(class Player *, class Actor *, class IItem *, uint32_t);
    virtual void SellItem(class Player *, class Actor *, class IItem *, uint32_t);
    virtual void Respawn(class Player *);
    virtual void Teleport(class Player *, const std::string &);
    virtual void Chat(class Player *, const std::string &);
    virtual void FastTravel(class Player *, const std::string &, const std::string &);
    virtual void SetPvPDesired(class Player *, bool);
    virtual void SubmitDLCKey(class Player *, const std::string &);
    virtual void SetCircuitInputs(class Player *, const std::string &, uint32_t);
    virtual void SendAddItemEvent(class Player *, class IItem *, uint32_t);
    virtual void SendRemoveItemEvent(class Player *, class IItem *, uint32_t);
    virtual void SendLoadedAmmoEvent(class Player *, class IItem *, uint32_t);
    virtual void SendPickedUpEvent(class Player *, const std::string &);
    virtual void EquipItem(class Player *, uint8_t, class IItem *);
    virtual void SetCurrentSlot(class Player *, uint8_t);
    virtual void SendEquipItemEvent(class Player *, uint8_t, class IItem *);
    virtual void SendCurrentSlotEvent(class Player *, uint8_t);
    virtual void SetCurrentQuest(class Player *, class IQuest *);
    virtual void SendSetCurrentQuestEvent(class Player *, class IQuest *);
    virtual void SendStartQuestEvent(class Player *, class IQuest *);
    virtual void SendAdvanceQuestToStateEvent(class Player *, class IQuest *, class IQuestState *);
    virtual void SendCompleteQuestEvent(class Player *, class IQuest *);
    virtual void SendHealthUpdateEvent(class Actor *, int32_t);
    virtual void SendManaUpdateEvent(class Player *, int32_t);
    virtual void SendCountdownUpdateEvent(class Player *, int32_t);
    virtual void SendPvPCountdownUpdateEvent(class Player *, bool, int32_t);
    virtual void SendPvPEnableEvent(class Player *, bool);
    virtual void SendStateEvent(class Actor *, const std::string &, bool);
    virtual void SendTriggerEvent(class Actor *, const std::string &, class Actor *, bool);
    virtual void SendFireBulletsEvent(class Actor *, class IItem *, const struct Vector3 &, uint32_t, float);
    virtual void SendDisplayEvent(class Player *, const std::string &, const std::string &);
    virtual void SendNPCConversationStateEvent(class Player *, class Actor *, const std::string &);
    virtual void SendNPCConversationEndEvent(class Player *);
    virtual void SendNPCShopEvent(class Player *, class Actor *);
    virtual void SendRespawnEvent(class Player *, const struct Vector3 &, const struct Rotation &);
    virtual void SendTeleportEvent(class Actor *, const struct Vector3 &, const struct Rotation &);
    virtual void SendRelativeTeleportEvent(class Actor *, const struct Vector3 &);
    virtual void SendReloadEvent(class Player *, class IItem *, class IItem *, uint32_t);
    virtual void SendPlayerJoinedEvent(class Player *);
    virtual void SendPlayerLeftEvent(class Player *);
    virtual void SendPlayerItemEvent(class Player *);
    virtual void SendActorSpawnEvent(class Actor *);
    virtual void SendActorDestroyEvent(class Actor *);
    virtual void SendExistingPlayerEvent(class Player *, class Player *);
    virtual void SendExistingActorEvent(class Player *, class Actor *);
    virtual void SendChatEvent(class Player *, const std::string &);
    virtual void SendKillEvent(class Player *, class Actor *, class IItem *);
    virtual void SendCircuitOutputEvent(class Player *, const std::string &, uint32_t, const class std::vector<class std::allocator<bool>> &);
    virtual void SendActorPositionEvents(class Player *);
    virtual void SendRegionChangeEvent(class Player *, const std::string &);
    virtual void SendLastHitByItemEvent(class Player *, class IItem *);
    bool SpawnActor(class Actor *, const struct Vector3 &, const struct Rotation &);
    bool SpawnActorAtNamedLocation(class Actor *, const char *);
    void SpawnActorWithId(uint32_t, class Actor *, const struct Vector3 &, const struct Rotation &);
    void DestroyActor(class Actor *);
    void SendSpawnEventsForExistingActors(class Player *);
    void AddAIZone(class AIZone *);
    class AIZone * GetAIZone(const std::string &);
    void OnPlayerEnteredAIZone(const std::string &);
    void OnPlayerLeftAIZone(const std::string &);
    class std::vector<IPlayer*, std::allocator<IPlayer*> > GetPlayersInRadius(const struct Vector3 &, float);
    class std::vector<Projectile*, std::allocator<Projectile*> > GetProjectilesInRadius(const struct Vector3 &, float);
    class Actor * GetActorById(uint32_t);
    void RemoveAllActorsExceptPlayer(class Player *);
    void ChangeActorId(class Player *, uint32_t);
    bool IsPlayerAlreadyConnected(uint32_t);
};