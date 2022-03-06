/**
 * @file    sdfwComponent.hpp
 * @author  Y.Nakaue
 */

#pragma once

 /**
  * @brief  Displayer component class
  */
template<class T>
class sdfwComponent
{
private:
    inline static T* pComponent = nullptr;

public:
    /**
     * @brief  Get component (return self pointer)
     */
    [[nodiscard]]
    T* get()
    {
        if (pComponent == nullptr)
        {
            pComponent = T::create();
        }

        return pComponent;
    }

    /**
     * @brief  Release this component
     */
    void release()
    {
        delete pComponent;
        pComponent = nullptr;
    }
};
