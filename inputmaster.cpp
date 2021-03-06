/* Masters of Oneiron
// Copyright (C) 2016 LucKey Productions (luckeyproductions.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "inputmaster.h"
#include "platform.h"
#include "oneirocam.h"
#include "slot.h"

InputMaster::InputMaster(Context* context) : Object(context)
{
    input_ = GetSubsystem<Input>();
    //Subscribe mouse down event
    SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(InputMaster, HandleMouseDown));
    //Subscribe key down event.
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(InputMaster, HandleKeyDown));
}

void InputMaster::HandleMouseDown(StringHash eventType, VariantMap &eventData)
{
    int button{ eventData[MouseButtonDown::P_BUTTON].GetInt() };
    if (button == MOUSEB_LEFT) {
        //See through cursor
        int first{0};
        if (MC->world.cursor.hitResults[first].node_->GetNameHash() == N_CURSOR)
            first = 1;

        if (firstHit_ = SharedPtr<Node>(MC->world.cursor.hitResults[first].node_)) {

            //Platform selection
            if (firstHit_->HasTag("Platform")) {

                Slot* slot{ firstHit_->GetComponent<Slot>() };
                //Building interaction, if platform was already selected
                //Slot interaction, if Former was selected
                if (slot && selectedPlatforms_.Contains(slot->GetPlatform()))
                {
                    SharedPtr<Platform> platform{ firstHit_->GetParentComponent<Platform>(true) };
                    IntVector2 coords{ IntVector2(firstHit_->GetComponent<Slot>()->coords_) };

                    if (platform->CheckEmpty(coords, true)) {
                        //Add tile
                        platform->AddTile(coords);
                        platform->FixFringe(/*coords*/);
                        platform->AddMissingSlots();
                    }
                    else {
                        //Add engine row
                        while (!platform->CheckEmpty(coords, true)){
                            platform->SetBuilding(coords, B_ENGINE);
                            coords += IntVector2(0, -1);
                        }
                    }
                } else if (input_->GetKeyDown(KEY_LSHIFT)||input_->GetKeyDown(KEY_RSHIFT)) {
                    //Add or remove platform to selection when either of the shift keys is held down
                    SharedPtr<Platform> platform{ firstHit_->GetParentComponent<Platform>(true) };

                    if (platform->IsSelected()) {
                        platform->SetSelected(false);
                        selectedPlatforms_.Remove(platform);
                    } else {
                        platform->SetSelected(true);
                        selectedPlatforms_ += platform;
                    }

                } else {
                //Select single platform
                    SharedPtr<Platform> platform{ firstHit_->GetParentComponent<Platform>(true) };
                    if (platform)
                        SetSelection(platform);
                }

            }

            //Void interaction (create new platform)
            else {
                SPAWN->Create<Platform>()->Set(MC->world.cursor.sceneCursor->GetPosition());
            }
        }
    }
    else if (button == MOUSEB_RIGHT){
        //Platform move command for each selected platform
        for (unsigned i = 0; i < selectedPlatforms_.Size(); i++){
            selectedPlatforms_[i]->SetMoveTarget(MC->world.cursor.sceneCursor->GetPosition());
        }
    }
}

void InputMaster::SetSelection(Platform* platform)
{
    DeselectAll();
    selectedPlatforms_ += platform;
    platform->Select();
}

void InputMaster::HandleMouseUp(StringHash eventType, VariantMap &eventData)
{
    using namespace MouseButtonUp;
    int button = eventData[P_BUTTON].GetInt();
    if (button == MOUSEB_LEFT) {}//Deselect when mouse did not move during click on N_VOID
}

void InputMaster::HandleKeyDown(StringHash eventType, VariantMap &eventData)
{
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();

    //Exit when ESC is pressed
    if (key == KEY_ESCAPE) DeselectAll();

    //Take screenshot
    else if (key == KEY_9)
    {
        Graphics* graphics = GetSubsystem<Graphics>();
        Image screenshot(context_);
        graphics->TakeScreenShot(screenshot);
        //Here we save in the Data folder with date and time appended
        String fileName{ GetSubsystem<FileSystem>()->GetProgramDir() + "Screenshots/Screenshot_" +
                Time::GetTimeStamp().Replaced(':', '_').Replaced('.', '_').Replaced(' ', '_')+".png" };
        Log::Write(1, fileName);
        screenshot.SavePNG(fileName);
    }
    else if (key == KEY_L)
    {
        Platform* platform{};
        if (firstHit_->GetNameHash() == N_TILEPART) platform = MC->platformMap_[firstHit_->GetParent()->GetParent()->GetID()];
        else if (firstHit_->GetNameHash() == N_SLOT) platform = MC->platformMap_[firstHit_->GetParent()->GetID()];
        if (platform) MC->world.camera->Lock(platform);
    }
}

void InputMaster::DeselectAll()
{
    for (unsigned i{ 0 }; i < selectedPlatforms_.Size(); i++)
    {
        selectedPlatforms_[i]->Deselect();
    }
    selectedPlatforms_.Clear();
}
