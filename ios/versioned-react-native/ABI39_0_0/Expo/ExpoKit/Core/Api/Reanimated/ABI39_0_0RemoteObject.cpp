#include "ABI39_0_0RemoteObject.h"
#include "ABI39_0_0SharedParent.h"
#include "ABI39_0_0NativeReanimatedModule.h"
#include <ABI39_0_0jsi/ABI39_0_0jsi.h>

using namespace ABI39_0_0facebook;

namespace ABI39_0_0reanimated {

void RemoteObject::maybeInitializeOnUIRuntime(jsi::Runtime &rt) {
  if (initializer.get() != nullptr) {
    backing = initializer->shallowClone(rt);
    initializer = nullptr;
  }
}

jsi::Value RemoteObject::get(jsi::Runtime &rt, const jsi::PropNameID &name) {
  if (module->isUIRuntime(rt)) {
    return backing->getProperty(rt, name);
  }
  return jsi::Value::undefined();
}

void RemoteObject::set(jsi::Runtime &rt, const jsi::PropNameID &name, const jsi::Value &value) {
  if (module->isUIRuntime(rt)) {
    backing->setProperty(rt, name, value);
  }
  // TODO: we should throw if trying to update remote from host runtime
}

std::vector<jsi::PropNameID> RemoteObject::getPropertyNames(jsi::Runtime &rt) {
  std::vector<jsi::PropNameID> res;
  auto propertyNames = backing->getPropertyNames(rt);
  for (size_t i = 0, size = propertyNames.size(rt); i < size; i++) {
    res.push_back(jsi::PropNameID::forString(rt, propertyNames.getValueAtIndex(rt, i).asString(rt)));
  }
  return res;
}

}
