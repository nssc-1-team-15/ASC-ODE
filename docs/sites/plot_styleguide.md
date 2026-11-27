# Plot style guide

Use this guide for plots created in ASC-ODE demos, documentation, and reports. The reference is the mass–spring phase portrait in `docs/pictures/massspring_phase.png`.

```{image} ../pictures/massspring_phase.png
:alt: Reference phase portrait: a blue solid numerical trajectory, a dark gray dashed exact solution, subtle grid, and a marked start point
:width: 70%
:align: center
```

## Visual principles

- Keep the plotting area white and uncluttered. Use a light gray grid to help readers estimate values without competing with the data.
- Use a clear blue for the computed numerical solution (`#2D7BE0` is a close match to the reference).
- Show reference or exact solutions in dark gray (`#555555`) with a dashed line. Use both color and line style to distinguish it from computed results.
- Use consistent, sufficiently thick lines: about `2.5` points for a main trajectory and `2.0` points for a comparison curve.
- Label every axis with the quantity and, where useful, its unit. Use concise sentence case; for example, `Time (s)`, `Position`, and `Velocity`.
- Add a legend when multiple series appear. Use descriptive names such as `Numerical solution` and `Exact solution`, not generic labels such as `data`.
- Mark initial conditions or important events directly on the plot when they help explain the solution. Keep annotations short and place them clear of the curves.
- Prefer explaining the result in the surrounding text or figure caption instead of adding a large title inside the axes. Add an axes title only when the plot must stand alone.

## Reuse the project style in Python

Use the shared helper in `../../plot` instead of copying style settings into each script. It defines the reference blue, neutral comparison color, and Matplotlib defaults. From a script in the repository, apply it before creating figures:

```python
from plot.style import BLUE, CHARCOAL, apply_plot_style

apply_plot_style()
```

Use `BLUE` for the computed numerical solution and `CHARCOAL` for exact or reference curves. Add project-wide plot settings to this module so all scripts can share them.

For a phase portrait, keep the numerical solution and exact/reference solution visually distinct and show the initial state:

```python
fig, ax = plt.subplots(figsize=(8, 8), layout="constrained")
ax.plot(position_num, velocity_num, color=BLUE, label="Numerical solution")
ax.plot(position_exact, velocity_exact, color=CHARCOAL, linestyle="--",
        linewidth=2.0, label="Exact solution")
ax.scatter(position_num[0], velocity_num[0], color=BLUE, s=55, zorder=3)
ax.annotate("Start", (position_num[0], velocity_num[0]),
            xytext=(8, 8), textcoords="offset points", color="#55534F")
ax.set_xlabel("Position")
ax.set_ylabel("Velocity")
ax.legend(loc="upper center", ncol=2, frameon=False)
```

Use equal axis scaling only when the axes represent commensurate quantities or when preserving geometric shape is important. Position and velocity have different units in a dimensional model, so equal scaling is a deliberate visual choice rather than a default.

## Comparing methods and plotting time histories

When comparing several numerical methods, keep the exact/reference solution dashed and neutral. Give each numerical method a distinct color from a color-blind-friendly palette, and use distinct line styles or markers as a second visual cue. Reuse each method's color consistently across figures. Label the method and, where it matters, the step size in the legend.

For time histories, use time on the horizontal axis and label each state variable with its name and units. If position and velocity have very different scales or units, use separate panels instead of putting them on one axis. Avoid smoothing or interpolation that could hide numerical behavior; plot the computed samples with lines and use markers when the step locations matter.

## Exporting figures

- Use PNG at `dpi=200` or higher for documentation and slides; use SVG or PDF for publication-quality vector output when the target supports it.
- Pass `bbox_inches="tight"` to avoid clipping labels and legends.
- Use a descriptive filename that identifies the system and plot, such as `mass_spring_phase.png` or `mass_spring_time_history.png`.
- Keep text readable at the final display size. Check that labels, legend entries, and line styles remain clear after the figure is embedded in a page.

Example:

```python
fig.savefig("mass_spring_phase.png", dpi=250, bbox_inches="tight")
```
