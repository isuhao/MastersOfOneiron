#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Input/Input.h>

#include "tile.h"
#include "imp.h"
#include "frop.h"
#include "grass.h"

Tile::Tile(Context *context, const IntVector2 coords, Platform *platform):
Object(context)
{
    masterControl_ = platform->masterControl_;
    platform_ = platform;
    coords_ = coords;

    rootNode_ = platform_->rootNode_->CreateChild("Tile");
    rootNode_->SetPosition(Vector3((double)coords_.x_, 0.0f, -(double)coords_.y_));
    //Increase platform mass
    platform_->rigidBody_->SetMass(platform_->rigidBody_->GetMass()+1.0f);
    //Add collision shape to platform
    collisionShape_ = platform->rootNode_->CreateComponent<CollisionShape>();
    collisionShape_->SetBox(Vector3::ONE);
    collisionShape_->SetPosition(Vector3(coords_.x_, 0.0f, -coords_.y_));
    //platform_->rigidBody_->EnableMassUpdate();

    //Create random tile addons
    int extraRandomizer = Random(23);

    //Create a dreamspire at random
    if (extraRandomizer == 7) {
        Node* spireNode = rootNode_->CreateChild("Spire");
        StaticModel* model = spireNode->CreateComponent<StaticModel>();
        model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Dreamspire.mdl"));
        model->SetMaterial(0,masterControl_->cache_->GetResource<Material>("Resources/Materials/solid.xml"));
        model->SetMaterial(1,masterControl_->cache_->GetResource<Material>("Resources/Materials/glass.xml"));
        model->SetCastShadows(true);
    }
    //Create random imps
    else if (extraRandomizer < 7){
        Vector3 randomPosition = Vector3(Random(-0.3f, 0.3f), 0.0f, Random(-0.3f, 0.3f));
        new Imp(context_, masterControl_, rootNode_, randomPosition);
    }
    //Create random plants
    else if (extraRandomizer > 8){
        for (int i = 0; i < extraRandomizer - 8; i++){
            Vector3 randomPosition = Vector3(Random(-0.4f, 0.4f), 0.0f, Random(-0.4f, 0.4f));
            new Frop(context_, masterControl_, rootNode_, randomPosition);
        }
    }

    /*for (int i = 0; i < (23-extraRandomizer)+16; i++){
        Vector3 randomPosition = Vector3(Random(-0.4f, 0.4f), 0.0f, Random(-0.4f, 0.4f));
        new Grass(context_, masterControl_, rootNode_, randomPosition);
    }*/

    //Set up center and edge nodes.
    for (int i = 0; i <= 8; i++){
        elements_[i] = rootNode_->CreateChild("TilePart");
        int nthOfType = ((i-1)%4);
        if (i > 0) elements_[i]->Rotate(Quaternion(0.0f, 90.0f-nthOfType*90.0f, 0.0f));
        //Add the right model to the node
        StaticModel* model = elements_[i]->CreateComponent<StaticModel>();
        switch (i){
        case TE_CENTER:    model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_center.mdl"));break;
        case TE_NORTH:case TE_EAST:case TE_SOUTH:case TE_WEST:
            model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_side.mdl"));break;
        case TE_NORTHEAST:case TE_SOUTHEAST:case TE_SOUTHWEST:case TE_NORTHWEST:
            model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_outcorner.mdl"));break;
                default:break;
        }

        model->SetMaterial(masterControl_->cache_->GetResource<Material>("Resources/Materials/block_center.xml"));
        model->SetCastShadows(true);
    }

    SubscribeToEvent(E_UPDATE, HANDLER(Tile, HandleUpdate));
}

void Tile::Start()
{
}
void Tile::Stop()
{
}

void Tile::HandleUpdate(StringHash eventType, VariantMap &eventData)
{
    Input* input = GetSubsystem<Input>();
    using namespace Update; double timeStep = eventData[P_TIMESTEP].GetFloat();
    if (buildingType_ == B_ENGINE && input->GetKeyDown(KEY_UP)){
        platform_->rigidBody_->ApplyForce(platform_->rootNode_->GetRotation() * rootNode_->GetDirection() * 500.0f*timeStep, platform_->rootNode_->GetRotation() * rootNode_->GetPosition());
    }
}

void Tile::SetBuilding(BuildingType type)
{
    Vector<SharedPtr<Node> > children = rootNode_->GetChildren();
    for (int i = 0; i < children.Length(); i++)
        if (children[i]->GetNameHash() != N_TILEPART)
            children[i]->SetEnabledRecursive(false);

    buildingType_ = type;
    if (buildingType_ > B_EMPTY) platform_->DisableSlot(coords_);
    StaticModel* model = elements_[0]->GetComponent<StaticModel>();
    switch (buildingType_)
    {
    case B_ENGINE: {
        model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Engine_center.mdl"));
        model->SetMaterial(0,masterControl_->cache_->GetResource<Material>("Resources/Materials/block_center.xml"));
        model->SetMaterial(1,masterControl_->cache_->GetResource<Material>("Resources/Materials/solid.xml"));
        model->SetMaterial(2,masterControl_->cache_->GetResource<Material>("Resources/Materials/glow.xml"));
        model->SetMaterial(3,masterControl_->cache_->GetResource<Material>("Resources/Materials/glass.xml"));
    } break;
    default: break;
    }
}

BuildingType Tile::GetBuilding()
{
    return buildingType_;
}

//Fix this tile's element models and materials according to
void Tile::FixFringe()
{
    for (int element = 1; element < TE_LENGTH; element++)
    {
        StaticModel* model = elements_[element]->GetComponent<StaticModel>();
        //Fix sides
        if (element <= 4){
            //If corresponding neighbour is empty
            if (platform_->CheckEmptyNeighbour(coords_, (TileElement)element, true))
            {
                if (element == 1)
                {
                    switch (buildingType_)
                    {
                    case B_ENGINE :
                        model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Engine_end.mdl"));
                        model->SetMaterial(0,masterControl_->cache_->GetResource<Material>("Resources/Materials/block_center.xml"));
                        model->SetMaterial(1,masterControl_->cache_->GetResource<Material>("Resources/Materials/solid.xml"));
                        model->SetMaterial(2,masterControl_->cache_->GetResource<Material>("Resources/Materials/glow.xml"));
                        model->SetMaterial(3,masterControl_->cache_->GetResource<Material>("Resources/Materials/glass.xml"));
                    break;
                    default: model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_side.mdl"));
                    break;
                    }
                }
                else if (element == 3)
                {
                    switch (buildingType_)
                    {
                    case B_ENGINE :
                        model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Engine_start.mdl"));
                        model->SetMaterial(2,masterControl_->cache_->GetResource<Material>("Resources/Materials/block_center.xml"));
                        model->SetMaterial(0,masterControl_->cache_->GetResource<Material>("Resources/Materials/solid.xml"));
                        model->SetMaterial(1,masterControl_->cache_->GetResource<Material>("Resources/Materials/glow.xml"));
                    break;
                    default: model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_side.mdl"));
                    break;
                    }
                }
                else model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_side.mdl"));
            }
            //If neighbour is not empty
            else {
                if (element == 1) {
                    switch (GetBuilding())
                    {
                    case B_ENGINE : if (platform_->GetNeighbourType(coords_, (TileElement)element) == B_ENGINE) model->SetModel(SharedPtr<Model>()); break;
                    default : model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_tween.mdl")); break;
                    }
                }
                else if (element == 4) {model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_tween.mdl"));}
                else model->SetModel(SharedPtr<Model>());
            }
        }
        //Fix corners
        else {
            switch (platform_->PickCornerType(coords_, (TileElement)element)){
            case CT_NONE:   model->SetModel(SharedPtr<Model>()); break;
            case CT_IN:     model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_incorner.mdl")); break;
            case CT_OUT:    model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_outcorner.mdl")); break;
            case CT_TWEEN:  model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_tweencorner.mdl")); break;
            case CT_DOUBLE: model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_doublecorner.mdl")); break;
            case CT_FILL:   model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Block_fillcorner.mdl")); break;
            default: break;
            }
            model->SetMaterial(masterControl_->cache_->GetResource<Material>("Resources/Materials/block_center.xml"));
        }
    }
}






















