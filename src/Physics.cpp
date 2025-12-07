
#include "Physics.hpp"

Physics::Physics(float gravity, float groundLevel, MC& mc)
    : gravity_(gravity), groundLevel_(groundLevel), velocity_(0, 0, 0) {

    // Bullet world setup
    broadphase_ = new btDbvtBroadphase();
    collisionConfig_ = new btDefaultCollisionConfiguration();
    dispatcher_ = new btCollisionDispatcher(collisionConfig_);
    solver_ = new btSequentialImpulseConstraintSolver();
    dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_, broadphase_, solver_, collisionConfig_);
    dynamicsWorld_->setGravity(btVector3(0, gravity_, 0));

    // Ground plane
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), groundLevel_);
    btDefaultMotionState* groundMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, groundLevel_, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    dynamicsWorld_->addRigidBody(new btRigidBody(groundCI));

    // Motorcycle rigid body
    btCollisionShape* mcShape = new btBoxShape(btVector3(1, 1, 1)); // Adjust size
    btVector3 startPos(mc.getPosition().x, mc.getPosition().y, mc.getPosition().z);
    btDefaultMotionState* mcMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), startPos));
    btScalar mass = 100;
    btVector3 inertia(0, 0, 0);
    mcShape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo mcCI(mass, mcMotionState, mcShape, inertia);
    mcRigidBody_ = new btRigidBody(mcCI);
    mcRigidBody_->setFriction(0.5f);
    dynamicsWorld_->addRigidBody(mcRigidBody_);
}

Physics::~Physics() {
    delete dynamicsWorld_;
    delete solver_;
    delete dispatcher_;
    delete collisionConfig_;
    delete broadphase_;
}

void Physics::applyAcceleration(MC& mc) {
    auto vel = mc.getSpeedVector(); // Use MC's speed and rotation
    mcRigidBody_->activate(true);   // Wake up the rigid body
    mcRigidBody_->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
}


void Physics::update(MC& mc, float deltaTime) {
    dynamicsWorld_->stepSimulation(deltaTime);
    btTransform trans;
    mcRigidBody_->getMotionState()->getWorldTransform(trans);
    auto pos = trans.getOrigin();
    mc.setPosition({pos.x(), pos.y(), pos.z()});
}

void Physics::setVelocity(const threepp::Vector3& vel) {
    velocity_ = vel;
    mcRigidBody_->activate(true);
    mcRigidBody_->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
}

const threepp::Vector3& Physics::getVelocity() const {
    return velocity_;
}

void Physics::resetVelocity() {
    velocity_ = {0, 0, 0};
    mcRigidBody_->activate(true);
    mcRigidBody_->setLinearVelocity(btVector3(0, 0, 0));
}
