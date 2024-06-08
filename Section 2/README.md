# Section 2: Disassembling, Decompiling and Debugging

Now that we have a working game we have completed the first step in Game Hacking - find a game to hack. We shall now go onto the second step in Game Hacking - Familiarise yourself with the game and look for vulnerabilities.

It is a good idea to take some time to play the game and explore some of the game mechanics so that you know what to look for while taking apart the game binary.

## Required tools

In this week we shall be mainly using two tools: [BinaryNinja](https://binary.ninja/free/) and [GDB](https://github.com/pwndbg/pwndbg). Ensure that you have downloaded both of them.

## Disassembling and Decompiling

But wait. What do we have to disassemble anyway? 

The first thought is of the executable `PwnAdventure3-Linux-Shipping`, since running that opens the game. However if you open it up in Binja (and wait for a long time for the analysis to complete) you will quickly realise that it doesn't contain much of the Game Logic. However searching "Game" in the top left search bar for Symbols does give us some intriguing functions.

![game search](https://imgur.com/PEWsDA6.png) 

However if you try to read the code of these functions (by double clicking on the entry) you will not find any code in the binary. Instead you will reach the `.extern` section of the binary. This section includes all the files that are imported by the binary. This suggests that the actual logic is present in some other file. Indeed, if you run the command `ldd PwnAdventure3-Linux-Shipping` you will find that it has included a file named "libGameLogic.so".

![libgamelogic](Screenshot_2024-06-06_20-57-06-mh.png)

Infact it is present in the same directory! Decompiling it in Binja, and searching for the keyword "Game" again will give us a lot more functions this time! Scrolling down a bit, you will find a lot of interesting functions.

![interesting funcs](https://imgur.com/aeY94qf.png)

Although these functions themselves don't contain any code, searching for their names separately will return a lot of related functions. For example this code of Player::Teleport seems to be calling some function defined in the GameWorld Class.

![teleport](https://imgur.com/H2QK12d.png)

This raises an important problem: the decompiler doesn't actually know the exact format of the classes defined in the binary. However, the debugger GDB can help us with this task!

### Data section:
Another really important section of the game binary is the data section. You can find this in the lowest part of the binary, just above the `.extern` section. The `.bss` section contains all of the uninitalized global variables of the program. 

![.bss](https://imgur.com/7zrRX4c.png)

As we can see there are variables named **Game**, **GameWorld**, **g_items** among other variables. These come in handy during the dynamic analysis part.

## Debugging

Until now, we have been performing what is called `static analysis` by running a decompiler on a file without actually executing it. This type of analysis reveals the fixed aspects of the binary, such as the code and data sections, by examining the compiled code. However, static analysis has its limitations.

Many crucial aspects of a program's behavior and structure are difficult to discern just by looking at the binary. One of these features is the internal classes and struct definitions.

To attach GDB to the game process, first run the game and then run this command:

```bash
gdb -p $(pidof ./PwnAdventure3-Linux-Shipping)
```

Now we can have a look at various Game Objects and their structures and member functions by the `ptype` and `p` commands.

For example, running `ptype World` will give the following output:
![ptype world](https://imgur.com/X8RRzCJ.png)

Looking at the functions defined in this class, we can get an idea of a lot of different classes and structs. Some of these are: `IPlayer`, `Player` and `Actor`. 

Now recall that we saw some global variables called **Game** and **GameWorld**. Printing those out in GDB gives us:

![p](https://imgur.com/LhuzzQ7.png)

What does this mean?
It means that the variable `GameWorld` is a `World` object and `Game` is a `GameAPI` object respectively.

Dereferencing them gives us a load of information about the game:

![dereference](https://imgur.com/8MqEO7F.png)

However, the first line tells us that this object has the vtable for a `ClientWorld` object instead of a `World` object as seen before. That means we need to cast the GameWorld variable to a ClientWorld pointer to access the full class attributes and member functions. 

![clientworld](https://imgur.com/sE9Poxc.png)

Now we can also access some of its attributes by using a `.` to access the members. Repeatedly accessing members can help us reach our Player object as well.

![player](https://imgur.com/ruFvcTx.png)

------



## Assignment

There are two parts of this assignment: Decompiler and Debugger

### Part 1

Explore the decompiled binary in your favourite decompiler and prepare a list of all Hostile Creatures<sup id="fnref1"><a href="#fn1" title="Jump to footnote">[1]</a></sup> in the game. You have to submit a text file with each line containing a separate name of a Hostile Creature.

<a id="fn1"></a>
[1]: A Hostile Creature is defined as an entity that can decrease the Player's Health.

### Part 2

Write the following code in a `.cpp` file and save it.

```cpp
#include <iostream>
#include "libGameLogic.h"

int main(){
    std::cout << "Header is Ready!";

}
```

Now create a `.h` file named `libGameLogic.h` in the same directory and populate it with the `World` class which you got through GDB.

<details>
  <summary>libGameLogic.h</summary>

  ```cpp
  class World {
  public:
    std::set<ActorRef<IPlayer>> m_players;
    std::set<ActorRef<IActor>> m_actors;
    std::map<unsigned int, ActorRef<IActor>> m_actorsById;
    ILocalPlayer *m_localPlayer;
    uint32_t m_nextId;
    std::map<const char*, AIZone*> m_aiZones;

    void AddActorToWorld(Actor *);
    void AddActorToWorldWithId(uint32_t, Actor *);
    void SendEventToAllPlayers(const WriteStream &);
    void SendEventToAllPlayersExcept(Player *, const WriteStream &);
  public:
    World(void);
    virtual ~World(void);
    virtual void Tick(float);
    virtual bool HasLocalPlayer(void);
    ILocalPlayer * GetLocalPlayer(void);
    virtual bool IsAuthority(void);
    virtual void AddLocalPlayer(Player *, ILocalPlayer *);
    virtual void AddRemotePlayer(Player *);
    virtual void AddRemotePlayerWithId(uint32_t, Player *);
    virtual void RemovePlayer(Player *);
    virtual void Use(Player *, Actor *);
    virtual void Activate(Player *, IItem *);
    virtual void Reload(Player *);
    virtual void Jump(bool);
    virtual void Sprint(bool);
    virtual void FireRequest(bool);
    virtual void TransitionToNPCState(Player *, const std::string &);
    virtual void BuyItem(Player *, Actor *, IItem *, uint32_t);
    virtual void SellItem(Player *, Actor *, IItem *, uint32_t);
    virtual void Respawn(Player *);
    virtual void Teleport(Player *, const std::string &);
    virtual void Chat(Player *, const std::string &);
    virtual void FastTravel(Player *, const std::string &, const std::string &);
    virtual void SetPvPDesired(Player *, bool);
    virtual void SubmitDLCKey(Player *, const std::string &);
    virtual void SetCircuitInputs(Player *, const std::string &, uint32_t);
    virtual void SendAddItemEvent(Player *, IItem *, uint32_t);
    virtual void SendRemoveItemEvent(Player *, IItem *, uint32_t);
    virtual void SendLoadedAmmoEvent(Player *, IItem *, uint32_t);
    virtual void SendPickedUpEvent(Player *, const std::string &);
    virtual void EquipItem(Player *, uint8_t, IItem *);
    virtual void SetCurrentSlot(Player *, uint8_t);
    virtual void SendEquipItemEvent(Player *, uint8_t, IItem *);
    virtual void SendCurrentSlotEvent(Player *, uint8_t);
    virtual void SetCurrentQuest(Player *, IQuest *);
    virtual void SendSetCurrentQuestEvent(Player *, IQuest *);
    virtual void SendStartQuestEvent(Player *, IQuest *);
    virtual void SendAdvanceQuestToStateEvent(Player *, IQuest *, IQuestState *);
    virtual void SendCompleteQuestEvent(Player *, IQuest *);
    virtual void SendHealthUpdateEvent(Actor *, int32_t);
    virtual void SendManaUpdateEvent(Player *, int32_t);
    virtual void SendCountdownUpdateEvent(Player *, int32_t);
    virtual void SendPvPCountdownUpdateEvent(Player *, bool, int32_t);
    virtual void SendPvPEnableEvent(Player *, bool);
    virtual void SendStateEvent(Actor *, const std::string &, bool);
    virtual void SendTriggerEvent(Actor *, const std::string &, Actor *, bool);
    virtual void SendFireBulletsEvent(Actor *, IItem *, const Vector3 &, uint32_t, float);
    virtual void SendDisplayEvent(Player *, const std::string &, const std::string &);
    virtual void SendNPCConversationStateEvent(Player *, Actor *, const std::string &);
    virtual void SendNPCConversationEndEvent(Player *);
    virtual void SendNPCShopEvent(Player *, Actor *);
    virtual void SendRespawnEvent(Player *, const Vector3 &, const Rotation &);
    virtual void SendTeleportEvent(Actor *, const Vector3 &, const Rotation &);
    virtual void SendRelativeTeleportEvent(Actor *, const Vector3 &);
    virtual void SendReloadEvent(Player *, IItem *, IItem *, uint32_t);
    virtual void SendPlayerJoinedEvent(Player *);
    virtual void SendPlayerLeftEvent(Player *);
    virtual void SendPlayerItemEvent(Player *);
    virtual void SendActorSpawnEvent(Actor *);
    virtual void SendActorDestroyEvent(Actor *);
    virtual void SendExistingPlayerEvent(Player *, Player *);
    virtual void SendExistingActorEvent(Player *, Actor *);
    virtual void SendChatEvent(Player *, const std::string &);
    virtual void SendKillEvent(Player *, Actor *, IItem *);
    virtual void SendCircuitOutputEvent(Player *, const std::string &, uint32_t, const std::vector<std::allocator<bool>> &);
    virtual void SendActorPositionEvents(Player *);
    virtual void SendRegionChangeEvent(Player *, const std::string &);
    virtual void SendLastHitByItemEvent(Player *, IItem *);
    bool SpawnActor(Actor *, const Vector3 &, const Rotation &);
    bool SpawnActorAtNamedLocation(Actor *, const char *);
    void SpawnActorWithId(uint32_t, Actor *, const Vector3 &, const Rotation &);
    void DestroyActor(Actor *);
    void SendSpawnEventsForExistingActors(Player *);
    void AddAIZone(AIZone *);
    AIZone * GetAIZone(const std::string &);
    void OnPlayerEnteredAIZone(const std::string &);
    void OnPlayerLeftAIZone(const std::string &);
    std::vector<IPlayer*> GetPlayersInRadius(const Vector3 &, float);
    std::vector<Projectile*> GetProjectilesInRadius(const Vector3 &, float);
    Actor * GetActorById(uint32_t);
    void RemoveAllActorsExceptPlayer(Player *);
    void ChangeActorId(Player *, uint32_t);
    bool IsPlayerAlreadyConnected(uint32_t);
};
  ```
</details>

If you try to compile your cpp file you will get a lot of errors regarding undeclared Classes and unimported libraries. Your task is to solve all these errors by importing the necessary library or:
1. Finding the class or struct structure in GDB using ptype
2. Copying that code into the header file <br>

Doing this iteratively will eventually include all classes and will compile the cpp file without any error messages. Submit the header file and the cpp file.