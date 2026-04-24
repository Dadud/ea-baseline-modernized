# Foundation Portability Batch 029 — Renderer/Runtime Leakage Inventory

## Goal

Document the most important renderer/backend leakage points currently blocking clean runtime/physics separation and future upstream convergence work.

This batch is an **inventory/classification batch**, not a broad refactor.

It focuses on three hot spots:
- `ww3d2`
- `Combat`
- `wwphys`

The purpose is to identify:
- the worst header/include bleed
- misclassified source groups
- the smallest safe first implementation cuts
- which couplings are cheap to cut vs genuinely architectural

---

## Summary

Batch 029 confirms that the biggest current technical knot is **renderer/backend bleed into runtime-facing code**.

There are three different patterns:

1. **`ww3d2` header-chain bleed**
   - content-facing or reusable-looking headers pull DX8 backend headers transitively
2. **misbucketed runtime files**
   - some `wwphys` and `Combat` files currently treated as runtime-oriented are actually renderer glue or presentation code
3. **real architectural coupling**
   - some `Combat` product-shell couplings are not safe to remove casually, especially `datasafe`

The result is that not all blockers should be treated the same:
- some are **easy reclassification or include-surface cuts**
- some are **medium boundary/extraction work**
- some are **deep product architecture and data-layout issues**

---

## 1. `ww3d2` leakage findings

### Critical bleed path

The most important header chain found in Batch 029 is:

```text
mesh.h
  -> dx8polygonrenderer.h
    -> dx8wrapper.h
      -> d3d8.h
```

This means a content-facing or runtime-adjacent include of `mesh.h` can drag the entire DX8 backend API surface into downstream targets.

A second severe path exists in:

```text
dynamesh.h
  -> dx8wrapper.h
    -> d3d8.h
```

### Why this matters

This explains why `Combat` and `wwphys` can hit DX8-era renderer surfaces even when they are not obviously trying to do renderer backend work.

### Lowest-risk first cut candidate

The best first header-surface cut identified in this audit is:
- break the direct `mesh.h -> dx8polygonrenderer.h` include chain
- replace it with a forward declaration or a narrower backend-only inclusion path

This is attractive because it reduces a **shared upstream leak** instead of only trimming symptoms downstream.

---

## 2. `wwphys` leakage findings

### Key discovery

Some files currently grouped as runtime simulation are better understood as renderer glue.

Worst offenders:
- `phys.cpp`
- `staticanimphys.cpp`

These files directly include DX8 renderer/backend headers and make substantial DX8 wrapper calls.

### Additional leakage

Direct Win32 header leaks were also found in files that look more core-like:
- `Path.cpp`
- `vistable.cpp`
- `pathsolve.cpp`

Some files grouped under math/serialization also reference renderer-facing headers such as `ww3d.h` or scene headers.

### Best first cut from `wwphys`

The strongest low-risk cut here is **reclassification**:
- move obvious renderer-glue files such as `phys.cpp/.h` and `staticanimphys.cpp/.h`
  from runtime-sim classification
  into scene/render-glue classification in CMake

This would make the current boundary more honest without source rewriting.

---

## 3. `Combat` leakage findings

### Two distinct coupling classes

Batch 029 found that `Combat` contains both:
1. **small renderer/timing/presentation leaks** that look cuttable
2. **deep product-shell/data-layout coupling** that should not be treated as a quick cleanup

### Deep coupling: `datasafe`

Several public gameplay headers include `../Commando/datasafe.h`, notably:
- `damage.h`
- `playerdata.h`
- `weaponmanager.h`

This is not just a utility dependency. The `safe_*` types are part of actual gameplay-facing struct layout.

### Meaning

This should be treated as a real architectural product-shell coupling, not an early cleanup target.

### Smaller cuttable leaks

Better early targets found in `Combat` include:
- `timemgr.cpp`
- `debug.cpp`

Also, some clearly renderer/presentation-heavy files live in `Combat/`, including:
- `backgroundmgr.*`
- `WeatherMgr.*`
- `weaponview.cpp`
- `messagewindow.cpp`

These should be treated as presentation/render-oriented, not pure gameplay core.

### Best first cut from `Combat`

The smallest safe implementation slice identified here is:
- remove or replace the `WW3D::Get_Frame_Time()` dependency in `timemgr.cpp`

That is tiny, bounded, and much safer than touching `datasafe`.

---

## 4. Cross-subsystem interpretation

The three audits fit together cleanly:

### `ww3d2`
contains a **shared upstream header leak** that contaminates downstream users.

### `wwphys`
contains **misclassified renderer-glue files** currently treated as runtime-facing.

### `Combat`
contains both:
- a few **tiny cuttable renderer/timer leaks**
- and some **non-trivial product-shell/data-layout coupling** that should stay deferred for now

That gives a practical near-term strategy:
- cut shared header bleed where possible
- reclassify obviously misbucketed files
- take only the smallest source-level cuts first
- do **not** casually attack `datasafe`

---

## Ranked first implementation-cut candidates

## 1. Reclassify obvious `wwphys` renderer-glue files in CMake

Move obvious renderer-glue files such as:
- `phys.cpp/.h`
- `staticanimphys.cpp/.h`

out of runtime-sim grouping and into scene/render-glue grouping.

### Why first
- minimal risk
- clarifies current truth immediately
- no source-level semantic change
- improves the honesty of the current portable/runtime boundary

## 2. Small `Combat` timing cut in `timemgr.cpp`

Replace or isolate the `WW3D::Get_Frame_Time()` dependency with a narrower timing path.

### Why second
- tiny source diff
- clearly bounded
- good proof that the leakage inventory can drive safe code cuts

## 3. Break the `mesh.h -> dx8polygonrenderer.h` include chain

Reduce the most important shared upstream leak in `ww3d2`.

### Why third
- highest leverage shared leak
- but a little riskier than pure reclassification or a tiny timing cut
- should follow immediately after the first low-risk cuts if they land cleanly

---

## Explicit non-goal from this batch

Do **not** try to remove `datasafe` from `Combat` headers as an early implementation slice.

Reason:
- it affects actual gameplay struct layout
- it is tied to anti-tamper strategy and product-shell concerns
- this is architecture work, not a cleanup diff

That coupling should be documented and revisited later under a real product/runtime separation plan.

---

## Recommended next batch outcome

The best immediate follow-on from Batch 029 is:

### Batch 030
A small implementation batch that does **both**:
1. reclassifies the obvious `wwphys` renderer-glue files in CMake
2. makes the tiny `Combat/timemgr.cpp` timing cut

If those land cleanly, the next high-value move should be:
3. attack the `ww3d2` `mesh.h` backend include chain as the first shared upstream leakage cut

---

## Verification expectations

For any follow-on implementation slice:
- update relevant CMake/doc classification
- run `python3 scripts/architecture/check_doc_sync.py`
- run the smallest meaningful affected-target verification
- keep the change bounded and evidence-backed

---

## Final takeaway

The repo is not blocked by one giant mystery.
It is blocked by a mix of:
- one major upstream header-surface problem in `ww3d2`
- several misclassified renderer-glue files in `wwphys`
- a few tiny removable leaks in `Combat`
- and one real product-shell coupling (`datasafe`) that should not be treated lightly

That is good news.
It means the next implementation slices can be small, honest, and strategic.