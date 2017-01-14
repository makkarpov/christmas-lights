#ifndef _EFFECT_DESCRIPTOR_H_
#define _EFFECT_DESCRIPTOR_H_

#include "config.h"
#include "leds.h"
#include "effect.h"

#define EFFECT_DESCRIPTOR(...) ::EffectDescriptorImpl<__VA_ARGS__>::descriptor

struct EffectDescriptorEntry {
	virtual Effect* create(Leds &_leds) const = 0;
};

template <class E> struct EffectDescriptorEntryImpl: public EffectDescriptorEntry {
	static_assert(sizeof(E) <= EFFECT_DATA_BUFFER, "Effect data buffer is too small");
	
	static const EffectDescriptorEntryImpl<E> instance;
	static const EffectDescriptorEntry *descriptor;
	
	EffectDescriptorEntryImpl() {}
	
	virtual Effect* create(Leds &_leds) const {
		return new E(_leds);
	}
};

template <class E> const EffectDescriptorEntryImpl<E> EffectDescriptorEntryImpl<E>::instance;
template <class E> const EffectDescriptorEntry *EffectDescriptorEntryImpl<E>::descriptor = 
			&EffectDescriptorEntryImpl<E>::instance;

struct EffectDescriptor {
	virtual int count() const = 0;
	virtual const EffectDescriptorEntry *get(int i) const = 0;
};

template <class ...Effects> struct EffectDescriptorImpl: public EffectDescriptor {
	static const EffectDescriptorEntry *effects[sizeof...(Effects)];
	static const EffectDescriptorImpl<Effects...> instance;
	static const EffectDescriptor *descriptor;
	
	EffectDescriptorImpl() {}
	
	virtual int count() const {
		return sizeof...(Effects);
	}
	
	virtual const EffectDescriptorEntry *get(int i) const {
		return effects[i];
	}
};

template <class ...Effects> const EffectDescriptorEntry *EffectDescriptorImpl<Effects...>::effects[] =
			{ EffectDescriptorEntryImpl<Effects>::descriptor... };
			
template <class ...Effects> const EffectDescriptorImpl<Effects...> EffectDescriptorImpl<Effects...>::instance;
template <class ...Effects> const EffectDescriptor *EffectDescriptorImpl<Effects...>::descriptor =
	&EffectDescriptorImpl<Effects...>::instance;

#endif // _EFFECT_DESCRIPTOR_H_
