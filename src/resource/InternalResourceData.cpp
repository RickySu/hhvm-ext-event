/*
 * File:   InternalResourceData.cpp
 * Author: ricky
 *
 * Created on 2014年5月14日, 下午 12:13
 */

#include "InternalResourceData.h"

namespace HPHP {
    IMPLEMENT_OBJECT_ALLOCATION(InternalResourceData)
    InternalResourceData::InternalResourceData(void *resource) {
        this->resource = resource;
    }

    void * InternalResourceData::getInternalResourceData() {
        return this->resource;
    }

    void InternalResourceData::setInternalResourceData(void *resource) {
        this->resource = resource;
    }
    void InternalResourceData::setObject(const Object &obj) {
        this->object = &obj;
    }

    const Object *InternalResourceData::getObject() {
        return object;
    }

    InternalResourceData::~InternalResourceData() {
    }

}